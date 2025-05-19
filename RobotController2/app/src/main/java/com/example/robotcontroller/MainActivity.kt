@file:Suppress("ControlFlowWithEmptyBody", "DEPRECATION")

package com.example.robotcontroller

import android.Manifest
import android.annotation.SuppressLint
import android.bluetooth.BluetoothAdapter
import android.bluetooth.BluetoothDevice
import android.bluetooth.BluetoothSocket
import android.content.pm.PackageManager
import android.os.Build
import android.os.Bundle
import android.text.InputType
import android.widget.Button
import android.widget.EditText
import android.widget.TextView
import android.widget.Toast
import androidx.activity.result.contract.ActivityResultContracts
import androidx.annotation.RequiresPermission
import androidx.appcompat.app.AlertDialog
import androidx.appcompat.app.AppCompatActivity
import androidx.core.content.ContextCompat
import java.io.IOException
import java.io.OutputStream
import java.util.UUID

class MainActivity : AppCompatActivity() {

    private val hc05 = "20:19:07:00:2E:69" // Adapter à ton module
    private val uUID: UUID = UUID.fromString("00001101-0000-1000-8000-00805F9B34FB")

    private var bluetoothAdapter: BluetoothAdapter? = null
    private var bluetoothSocket: BluetoothSocket? = null
    private var outputStream: OutputStream? = null

    private lateinit var messageTextView: TextView

    @SuppressLint("MissingPermission")
    private val requestPermissionsLauncher = registerForActivityResult(
        ActivityResultContracts.RequestMultiplePermissions()
    ) @RequiresPermission(Manifest.permission.BLUETOOTH_CONNECT) { permissions ->
        if (permissions.entries.all { it.value }) {
            connectBluetooth()
        } else {
            toast("Permissions nécessaires")
        }
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        messageTextView = findViewById(R.id.messageTextView)
        bluetoothAdapter = BluetoothAdapter.getDefaultAdapter()

        if (bluetoothAdapter == null) {
            toast("Bluetooth non supporté")
            finish()
            return
        }

        checkPermissionsAndConnect()
        setupButtons()
    }

    private fun checkPermissionsAndConnect() {
        val permissionsNeeded = mutableListOf<String>()

        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.S) {
            if (ContextCompat.checkSelfPermission(this, Manifest.permission.BLUETOOTH_CONNECT) != PackageManager.PERMISSION_GRANTED)
                permissionsNeeded.add(Manifest.permission.BLUETOOTH_CONNECT)
            if (ContextCompat.checkSelfPermission(this, Manifest.permission.BLUETOOTH_SCAN) != PackageManager.PERMISSION_GRANTED)
                permissionsNeeded.add(Manifest.permission.BLUETOOTH_SCAN)
        } else {
            if (ContextCompat.checkSelfPermission(this, Manifest.permission.ACCESS_FINE_LOCATION) != PackageManager.PERMISSION_GRANTED)
                permissionsNeeded.add(Manifest.permission.ACCESS_FINE_LOCATION)
        }

        if (permissionsNeeded.isNotEmpty()) {
            requestPermissionsLauncher.launch(permissionsNeeded.toTypedArray())
        } else {
            connectBluetooth()
        }
    }

    @RequiresPermission(Manifest.permission.BLUETOOTH_CONNECT)
    private fun connectBluetooth() {
        if (!bluetoothAdapter!!.isEnabled) {
            appendMessage("Bluetooth désactivé. L’application fonctionne sans connexion.")
            return
        }

        try {
            val device: BluetoothDevice = bluetoothAdapter!!.getRemoteDevice(hc05)
            bluetoothSocket = device.createRfcommSocketToServiceRecord(uUID)
            bluetoothSocket?.connect()
            outputStream = bluetoothSocket?.outputStream
            toast("Connecté au HC-05")
            appendMessage("Connecté au module Bluetooth.")
        } catch (e: IOException) {
            e.printStackTrace()
            appendMessage("Erreur de connexion Bluetooth : ${e.message}")
        }
    }

    private fun setupButtons() {
        val commands = mapOf(
            R.id.btnStop to '0',
            R.id.btnAvancer to '1',
            R.id.btnSuiviLigne to 'B',
            R.id.btnGauche to '3',
            R.id.btnRetour to '4',
            R.id.btnDroite to '2',
            R.id.btnOuvrir to '5',
            R.id.btnFermer to '6',
            R.id.btnLuminosite to 'A',
            R.id.btnMonter to '7',
            R.id.btnDescendre to '8',
            R.id.btnLireCouleur to '9',
            R.id.btnComparer to 'E',
            R.id.btnSequence to 'C' // Le bouton "Séquence"
        )
        commands.forEach { (id, cmd) ->
            findViewById<Button>(id).setOnClickListener {
                sendCommand(cmd)
            }
        }

        findViewById<Button>(R.id.btnChangerDistance).setOnClickListener { showSeuilDialog() }
    }

    private fun sendCommand(command: Char) {
        try {
            val bytes = byteArrayOf(command.code.toByte())
            outputStream?.write(bytes)
            appendMessage("Commande envoyée : $command")
        } catch (e: IOException) {
            e.printStackTrace()
            toast("Erreur d’envoi de commande")
        }
    }

    private fun sendCommandString(cmd: String) {
        try {
            outputStream?.write(cmd.toByteArray())
        } catch (e: IOException) {
            e.printStackTrace()
            toast("Erreur d’envoi")
        }
    }

    private fun showSeuilDialog() {
        val input = EditText(this)
        input.inputType = InputType.TYPE_CLASS_NUMBER
        AlertDialog.Builder(this)
            .setTitle("Modifier le seuil")
            .setView(input)
            .setPositiveButton("OK") { dialog, _ ->
                val value = input.text.toString()
                if (value.isNotEmpty()) {
                    sendCommandString("D$value")
                    appendMessage("Seuil changé à $value")
                }
                dialog.dismiss()
            }
            .setNegativeButton("Annuler") { dialog, _ -> dialog.dismiss() }
            .show()
    }

    private fun appendMessage(msg: String) {
        messageTextView.append("\n$msg")
    }

    private fun toast(msg: String) {
        Toast.makeText(this, msg, Toast.LENGTH_SHORT).show()
    }

    override fun onDestroy() {
        super.onDestroy()
        try {
            bluetoothSocket?.close()
        } catch (e: IOException) {
            e.printStackTrace()
        }
    }
}
