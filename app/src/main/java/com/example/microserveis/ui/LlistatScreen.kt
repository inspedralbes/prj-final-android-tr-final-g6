package com.example.microserveis.ui

import android.graphics.drawable.Drawable
import androidx.compose.foundation.background
import androidx.compose.foundation.layout.Arrangement
import androidx.compose.runtime.Composable
import androidx.compose.ui.Modifier
import androidx.lifecycle.viewmodel.compose.viewModel
import androidx.compose.runtime.collectAsState
import androidx.compose.runtime.getValue
import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.Row
import androidx.compose.foundation.layout.fillMaxWidth
import androidx.compose.foundation.layout.padding
import androidx.compose.foundation.layout.size
import androidx.compose.foundation.lazy.LazyColumn
import androidx.compose.foundation.lazy.items
import androidx.compose.foundation.shape.CircleShape
import androidx.compose.material.icons.Icons
import androidx.compose.material.icons.filled.Settings
import androidx.compose.material3.Button
import androidx.compose.material3.Card
import androidx.compose.material3.CardDefaults
import androidx.compose.material3.Icon
import androidx.compose.material3.IconButton
import androidx.compose.material3.Text
import androidx.compose.ui.draw.clip
import androidx.compose.ui.graphics.Color.Companion.Green
import androidx.compose.ui.graphics.Color.Companion.Red
import androidx.compose.ui.res.painterResource
import androidx.compose.ui.text.style.TextAlign
import androidx.compose.ui.tooling.preview.Preview
import androidx.compose.ui.unit.dp
import androidx.compose.ui.unit.sp
import com.example.microserveis.R
import com.example.microserveis.data.Servei
import com.example.microserveis.data.ServeiUiState

@Composable
fun LlistatScreen(
    serveisViewModel: MicroServeiViewModel = viewModel(),
    modifier: Modifier = Modifier,
    ChangeState: (String) -> Unit = {},
    goToLogs: (String) -> Unit = {},
    goToErrorLogs: (String) -> Unit = {}
) {
    val uiState by serveisViewModel.uiState.collectAsState()
    val serveiUiState by serveisViewModel.serveiUiState.collectAsState()

    when(serveiUiState){
        is ServeiUiState.Loading -> {
            Text(text = "Carregant...")
        }
        is ServeiUiState.Error -> {
            Text(text = "Error")
        }
        is ServeiUiState.Success -> {

            LazyColumn(modifier = Modifier.padding(16.dp)) {
                items(uiState.serveis) { service ->
                    ServiceCard(service, ChangeState, goToLogs, goToErrorLogs)
                }
            }
        }
    }
}

@Composable
fun ServiceCard(
    service: Servei,
    ChangeState: (String) -> Unit = {},
    goToLogs: (String) -> Unit = {},
    goToErrorLogs: (String) -> Unit = {},
    modifier: Modifier = Modifier
) {
    Card(
        modifier = modifier
            .padding(16.dp)
            .fillMaxWidth(),
        elevation = CardDefaults.cardElevation(defaultElevation = 2.dp)
    ) {
        Row(
            modifier = Modifier
                .fillMaxWidth()
                .padding(vertical = 8.dp),
            horizontalArrangement = Arrangement.SpaceAround,
            verticalAlignment = androidx.compose.ui.Alignment.CenterVertically
        ) {
            Text(
                text = service.name,
                textAlign = TextAlign.Center,
                fontSize = 20.sp
            )

            IconButton(
                onClick = { ChangeState(service.id) },
                modifier = Modifier
                    .size(48.dp)
                    .background(color = if (service.state == "tancat") Red else Green, shape = CircleShape)
                    .clip(CircleShape)
            ) {
                Icon(
                    painter = painterResource(id = if (service.state == "tancat") R.drawable.baseline_power_settings_new_24_red else R.drawable.baseline_power_settings_new_24_green),
                    contentDescription = if (service.state == "tancat") "Encendre" else "Tancar"
                )
            }
        }

        Row(
            modifier = Modifier
                .fillMaxWidth()
                .padding(vertical = 8.dp),
            horizontalArrangement = Arrangement.SpaceEvenly
        ) {

            Button(onClick = { goToLogs(service.id) }) {
                Text(text = "Logs")
            }
            Button(onClick = { goToErrorLogs(service.id) }) {
                Text(text = "Error Logs")
            }
        }
    }
}

@Preview
@Composable
fun ServiceCardPreview() {
    ServiceCard(Servei("1", "Test Service", "tancat"))
}