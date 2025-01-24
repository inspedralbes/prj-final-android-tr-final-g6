package com.example.microserveis.ui

import android.app.Application
import android.content.Context
import android.util.Log
import androidx.compose.ui.graphics.Color
import androidx.compose.ui.graphics.toArgb
import androidx.compose.ui.platform.LocalContext
import androidx.lifecycle.AndroidViewModel
import androidx.lifecycle.ViewModel
import androidx.lifecycle.viewModelScope
import androidx.room.Room
import com.example.microserveis.communications.BaseUrlHolder
import com.example.microserveis.communications.ServeiApi
import com.example.microserveis.data.AppDatabase
import com.example.microserveis.data.AppUiState
import com.example.microserveis.data.HostConfiguration
import com.example.microserveis.data.Servei
import com.example.microserveis.data.ServeiUiState
import com.example.microserveis.data.ServeiAAfectar
import com.google.gson.Gson
import io.socket.client.IO
import io.socket.client.Socket
import io.socket.emitter.Emitter
import kotlinx.coroutines.flow.MutableStateFlow
import kotlinx.coroutines.flow.StateFlow
import kotlinx.coroutines.flow.asStateFlow
import kotlinx.coroutines.flow.update
import kotlinx.coroutines.launch
import java.net.MalformedURLException
import java.net.URISyntaxException


class MicroServeiViewModel(application: Application) : AndroidViewModel(application) {

    val applicationContext = getApplication<Application>().applicationContext



    private val _uiState = MutableStateFlow(AppUiState())
    val uiState: StateFlow<AppUiState> = _uiState.asStateFlow()

    private val _serveiUiState = MutableStateFlow<ServeiUiState>(ServeiUiState.Loading)
    val serveiUiState: StateFlow<ServeiUiState> = _serveiUiState.asStateFlow()

    val gson = Gson()

    val db = Room.databaseBuilder(
        applicationContext,
        AppDatabase::class.java,
        "host-configurations"
    ).build()

    lateinit var mSocket: Socket

    fun getServeis() {
        viewModelScope.launch {
            _serveiUiState.value = ServeiUiState.Loading
            try {
                val serveis = ServeiApi.retrofitService.getServeis()

                _uiState.update { currentState ->
                    currentState.copy(
                        serveis = serveis
                    )
                }

                _serveiUiState.value = ServeiUiState.Success(serveis)
            } catch (e: Exception) {
                println("error: ${e.message}")
                _serveiUiState.value = ServeiUiState.Error
            }
        }
    }

    private fun connectSocket() {
        try {
            mSocket = IO.socket(BaseUrlHolder.baseUrl)
        } catch (e: Exception) {
            e.printStackTrace()
            Log.e("SocketIO", "Failed to connect to socket", e)
        }
        mSocket.connect()
        mSocket.on(Socket.EVENT_CONNECT) {
            println("Connected to host: ${BaseUrlHolder.baseUrl}")
            Log.d("SocketIO", "Connected to socket: ${mSocket.id()}")
            mSocket.on("actualitzar serveis", actualitzarServeis)
        }
        mSocket.on(Socket.EVENT_DISCONNECT) {
            Log.d("SocketIO", "Disconnected from socket")
        }
    }

    private fun reconnectSocket() {
        if (::mSocket.isInitialized && mSocket.connected()) {
            mSocket.disconnect()
            mSocket.off() // Remove all listeners
        }

        try {
            mSocket = IO.socket(BaseUrlHolder.baseUrl)
        } catch (e: URISyntaxException) {
            e.printStackTrace()
            Log.e("SocketIO", "Failed to connect to socket", e)
        }
        mSocket.connect()
        mSocket.on(Socket.EVENT_CONNECT) {
            println("Connected to host: ${BaseUrlHolder.baseUrl}")
            Log.d("SocketIO", "Connected to socket: ${mSocket.id()}")
            mSocket.on("actualitzar serveis", actualitzarServeis)
        }
        mSocket.on(Socket.EVENT_DISCONNECT) {
            Log.d("SocketIO", "Disconnected from socket")
        }
    }

    private val actualitzarServeis = Emitter.Listener { args ->
        val serveisJson = args[0] as String

        val serveis = gson.fromJson(serveisJson, Array<Servei>::class.java).toList()

        _uiState.update { currentState ->
            currentState.copy(
                serveis = serveis
            )
        }
    }

    fun changeState(id: String) {
        viewModelScope.launch {
            try {
                val serveis = ServeiApi.retrofitService.canviarEstat(ServeiAAfectar(id))
                _uiState.update { currentState ->
                    currentState.copy(
                        serveis = serveis
                    )
                }
            } catch (e: Exception) {
                println("error: ${e.message}")
            }
        }
    }

    fun setLogging(id: String) {
        _uiState.update { currentState ->
            currentState.copy(
                currentlyLogging = id
            )
        }
    }

    fun getHostsFromDB() {
        viewModelScope.launch {
            getAllHost()
        }
    }

    fun insertHost(newHostConfig: HostConfiguration) {

        println("Inserting host configuration: $newHostConfig")


        viewModelScope.launch {
            addHostConfiguration(newHostConfig)
        }
    }

    private suspend fun addHostConfiguration(newHostConfig: HostConfiguration) {

        if (validateHostConfiguration(newHostConfig)) {
            db.hostConfigurationDao().insert(newHostConfig)
            println("Host configuration inserted with ID: ${newHostConfig.id}")

        } else {
            println("Invalid host configuration: $newHostConfig")
        }


    }

    private fun validateHostConfiguration(hostConfiguration: HostConfiguration): Boolean {
        return with(hostConfiguration) {
            name.isNotEmpty() &&
                    color != Color.Unspecified.toArgb() && // Check if color is not Unspecified
                    host.isNotEmpty() &&
                    port.isNotEmpty()
        }
    }

    suspend fun deleteAllHost() {
        db.hostConfigurationDao().deleteAll()
        println("All host configurations deleted")
    }

    suspend fun getAllHost() {
        val hostConfigurations = db.hostConfigurationDao().getAll()

        println("All host configurations:")

        hostConfigurations.forEach { hostConfig ->
            println("ID: ${hostConfig.id}, Name: ${hostConfig.name}, Color: ${hostConfig.color}, Host: ${hostConfig.host}, Port: ${hostConfig.port}")
        }

        _uiState.update { currentState ->
            currentState.copy(
                hostConfigurations = hostConfigurations
            )
        }
    }

    fun updateBaseUrl(hostConfiguration: HostConfiguration) {
        println("New URL: ${hostConfiguration.host}:${hostConfiguration.port}")
        BaseUrlHolder.baseUrl = hostConfiguration.host + ":" + hostConfiguration.port

        reconnectSocket()
    }

    init {
        getServeis()

        connectSocket()

        viewModelScope.launch {
//            insertHost()

            getAllHost()

//            deleteAllHost()
        }


    }
}