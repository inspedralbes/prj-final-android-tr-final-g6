package com.example.microserveis.data

import kotlinx.serialization.Serializable
import androidx.compose.ui.graphics.Color


data class AppUiState (
    val serveis: List<Servei> = listOf(),
    val currentlyLogging: String = "",
    val hostConfigurations: List<HostConfiguration> = listOf()
)

@Serializable
data class Servei (
    val id: String = "",
    val name: String = "",
    val state: String = "",
    val logs: List<Log> = listOf(),
    val errorLogs: List<Log> = listOf(),
)

@Serializable
data class Log (
    val date: String = "",
    val log: String = "",
)

data class ColorToSelect (
    val name: String = "",
    val color: Color = Color.White,

    )

data class ServeiAAfectar(
    val id: String = "",
)

sealed interface ServeiUiState {
    data class Success(val servei: List<Servei>) : ServeiUiState
    object Error : ServeiUiState
    object Loading : ServeiUiState
}
