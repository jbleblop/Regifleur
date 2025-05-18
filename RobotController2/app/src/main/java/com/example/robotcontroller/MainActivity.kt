package com.example.robotcontroller

import android.Manifest
import android.bluetooth.BluetoothAdapter
import android.bluetooth.BluetoothDevice
import android.bluetooth.BluetoothSocket
import android.content.pm.PackageManager
import android.os.Build
import android.os.Bundle
import android.widget.Button
import android.widget.TextView
import android.widget.Toast
import androidx.activity.result.contract.ActivityResultContracts
import androidx.appcompat.app.AlertDialog
import androidx.appcompat.app.AppCompatActivity
import androidx.core.content.ContextCompat
import java.io.IOException
import java.io.OutputStream
import java.util.*

class MainActivity : AppCompatActivity() {

    private val HC05_ADDRESS = "00:00:00:00:00:00" // Remplace avec ton adresse HC-05
    private val UUID_SPP: UUID = UUID.fromString("00001101-0000-1000-8000-00805F9B34FB")

    private var bluetoothAdapter: BluetoothAdapter? = null
    private var bluetoothSocket: BluetoothSocket? = null
    private var outputStream: OutputStream? = null

    private lateinit var messageTextView: TextView

    private var langueFr = true

    // Gestion runtime permissions Bluetooth (Android 12+)
    private val requestPermissionLauncher = registerForActivityResult(
        ActivityResultContracts.RequestMultiplePermissions()
    ) { permissions ->
        val granted = permissions.entries.all { it.value }
        if (!granted) {
            toast("Permissions Bluetooth nécessaires")
            finish()
        } else {
            initBluetoothConnection()
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
        if (!bluetoothAdapter!!.isEnabled) {
            toast("Activez le Bluetooth et relancez l'application")
            finish()
            return
        }

        checkBluetoothPermissions()
        setupButtons()
    }

    private fun checkBluetoothPermissions() {
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.S) {
            val neededPermissions = arrayOf(
                Manifest.permission.BLUETOOTH_CONNECT,
                Manifest.permission.BLUETOOTH_SCAN
            )
            val granted = neededPermissions.all { perm ->
                ContextCompat.checkSelfPermission(this, perm) == PackageManager.PERMISSION_GRANTED
            }
            if (!granted) {
                requestPermissionLauncher.launch(neededPermissions)
                return
            }
        }
        // Permissions ok ou Android <12
        initBluetoothConnection()
    }

    private fun initBluetoothConnection() {
        val device: BluetoothDevice? = try {
            bluetoothAdapter?.getRemoteDevice(HC05_ADDRESS)
        } catch (e: IllegalArgumentException) {
            toast("Adresse HC-05 invalide")
            finish()
            return
        }
        try {
            bluetoothSocket = device?.createRfcommSocketToServiceRecord(UUID_SPP)
            bluetoothSocket?.connect()
            outputStream = bluetoothSocket?.outputStream
            toast("Connecté au HC-05")
        } catch (e: IOException) {
            e.printStackTrace()
            toast("Erreur de connexion Bluetooth")
            finish()
        }
    }

    private fun setupButtons() {
        findViewById<Button>(R.id.btnStop).setOnClickListener { sendCommand('0') }
        findViewById<Button>(R.id.btnAvancer).setOnClickListener { sendCommand('1') }
        findViewById<Button>(R.id.btnSuiviLigne).setOnClickListener { sendCommand('B') }

        findViewById<Button>(R.id.btnGauche).setOnClickListener { sendCommand('3') }
        findViewById<Button>(R.id.btnRetour).setOnClickListener { sendCommand('4') }
        findViewById<Button>(R.id.btnDroite).setOnClickListener { sendCommand('2') }

        findViewById<Button>(R.id.btnOuvrir).setOnClickListener { sendCommand('5') }
        findViewById<Button>(R.id.btnFermer).setOnClickListener { sendCommand('6') }
        findViewById<Button>(R.id.btnLuminosite).setOnClickListener { sendCommand('A') }

        findViewById<Button>(R.id.btnMonter).setOnClickListener { sendCommand('7') }
        findViewById<Button>(R.id.btnDescendre).setOnClickListener { sendCommand('8') }
        findViewById<Button>(R.id.btnLireCouleur).setOnClickListener { sendCommand('9') }

        findViewById<Button>(R.id.btnComparer).setOnClickListener { sendCommand('E') }

        findViewById<Button>(R.id.btnLangue).setOnClickListener {
            toggleLanguage()
        }

        findViewById<Button>(R.id.btnChangerDistance).setOnClickListener {
            showSeuilDialog()
        }
    }

    private fun sendCommand(command: Char) {
        try {
            outputStream?.write(command.toInt())
            appendMessage("Commande envoyée : $command")
        } catch (e: IOException) {
            e.printStackTrace()
            toast("Erreur d'envoi")
        }
    }

    private fun sendCommand(command: String) {
        try {
            outputStream?.write(command.toByteArray())
            appendMessage("Commande envoyée : $command")
        } catch (e: IOException) {
            e.printStackTrace()
            toast("Erreur d'envoi")
        }
    }

    private fun appendMessage(msg: String) {
        messageTextView.append("\n$msg")
    }

    private fun toast(message: String) {
        Toast.makeText(this, message, Toast.LENGTH_SHORT).show()
    }

    private fun toggleLanguage() {
        langueFr = !langueFr
        val langue = if (langueFr) "Français" else "English"
        appendMessage("Langue changée : $langue")
        // Ici tu peux modifier les textes des boutons selon la langue si tu veux
    }

    private fun showSeuilDialog() {
        val builder = AlertDialog.Builder(this)
        builder.setTitle("Modifier le seuil")

        val input = android.widget.EditText(this)
        input.inputType = android.text.InputType.TYPE_CLASS_NUMBER
        builder.setView(input)

        builder.setPositiveButton("OK") { dialog, _ ->
            val valeur = input.text.toString()
            if (valeur.isNotEmpty()) {
                sendCommand('D')
                sendCommand("$valeur\n") // Envoi du seuil
                appendMessage("Seuil changé à $valeur")
            }
            dialog.dismiss()
        }

        builder.setNegativeButton("Annuler") { dialog, _ -> dialog.cancel() }

        builder.show()
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
