package com.example.microserveis.ui

import androidx.compose.runtime.Composable
import androidx.compose.ui.Modifier

import androidx.lifecycle.viewmodel.compose.viewModel
import androidx.compose.runtime.collectAsState
import androidx.compose.runtime.getValue
import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.padding
import androidx.compose.foundation.lazy.LazyColumn
import androidx.compose.foundation.lazy.LazyRow
import androidx.compose.foundation.lazy.items
import androidx.compose.material3.Text
import androidx.compose.ui.unit.dp
import androidx.compose.ui.unit.sp

@Composable
fun LogsScreen(
    logsViewModel: MicroServeiViewModel = viewModel(),
    modifier: Modifier = Modifier,
    mode: String = "logs"
) {
    val uiState by logsViewModel.uiState.collectAsState()

    val serveiAMostrar = uiState.serveis.find { it.id == uiState.currentlyLogging }

    val isLogs = mode == "logs"

    if (serveiAMostrar != null) {

        Column {
            val textToShow = if (isLogs) "Logs" else "Error Logs"
            Text(
                text = textToShow + " del micro servei ${serveiAMostrar.name}",
                fontSize = 20.sp,
                modifier = Modifier.padding(8.dp)
            )
            val itemsToDisplay = if (isLogs) serveiAMostrar.logs else serveiAMostrar.errorLogs
            LazyColumn {
                items(itemsToDisplay) { text ->
                    LazyRow {
                        item {
                            Text(
                                text = text.date + " - " + text.log,
                                modifier = Modifier
//                            .padding(8.dp) // Add padding around each text item
                            )
                        }
                    }
                }
            }
        }
    } else {
        Text(text = "No se ha seleccionado ningún micro servicio")
    }
}


//                text = "Logs d'error del micro servei ${serveiAMostrar.name}",
