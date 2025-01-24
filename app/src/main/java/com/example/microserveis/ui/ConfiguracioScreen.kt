package com.example.microserveis.ui

import androidx.compose.foundation.background
import androidx.compose.foundation.layout.Box
import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.Row
import androidx.compose.foundation.layout.Spacer
import androidx.compose.foundation.layout.fillMaxWidth
import androidx.compose.foundation.layout.padding
import androidx.compose.foundation.layout.size
import androidx.compose.foundation.layout.width
import androidx.compose.foundation.lazy.LazyColumn
import androidx.compose.foundation.lazy.items
import androidx.compose.material3.AlertDialog
import androidx.compose.material3.Button
import androidx.compose.material3.Card
import androidx.compose.material3.CardDefaults
import androidx.compose.material3.OutlinedTextField
import androidx.compose.material3.Text
import androidx.compose.runtime.Composable
import androidx.compose.ui.Modifier
import androidx.lifecycle.viewmodel.compose.viewModel
import androidx.compose.runtime.collectAsState
import androidx.compose.runtime.getValue
import androidx.compose.runtime.mutableStateOf
import androidx.compose.runtime.remember
import androidx.compose.runtime.setValue
import androidx.compose.ui.Alignment
import androidx.compose.ui.graphics.Color
import androidx.compose.ui.text.font.FontWeight
import androidx.compose.ui.unit.dp
import androidx.core.text.color
import com.example.microserveis.data.HostConfiguration
import android.os.Bundle
import androidx.activity.ComponentActivity
import androidx.activity.compose.setContent
import androidx.compose.foundation.background
import androidx.compose.foundation.border
import androidx.compose.foundation.clickable
import androidx.compose.foundation.interaction.MutableInteractionSource
import androidx.compose.foundation.layout.Arrangement
import androidx.compose.foundation.layout.Box
import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.fillMaxHeight
import androidx.compose.foundation.layout.fillMaxWidth
import androidx.compose.foundation.layout.heightIn
import androidx.compose.foundation.layout.padding
import androidx.compose.foundation.layout.width
import androidx.compose.foundation.rememberScrollState
import androidx.compose.foundation.shape.CircleShape
import androidx.compose.foundation.verticalScroll
import androidx.compose.material.ripple.rememberRipple
import androidx.compose.material3.Divider
import androidx.compose.material3.HorizontalDivider
import androidx.compose.material3.Icon
import androidx.compose.material3.Text
//import androidx.compose.runtime.Composable
import androidx.compose.runtime.getValue
import androidx.compose.runtime.mutableStateOf
import androidx.compose.runtime.saveable.rememberSaveable
import androidx.compose.runtime.setValue
import androidx.compose.ui.graphics.toArgb
import androidx.compose.ui.res.painterResource
//import androidx.compose.ui.Alignment
//import androidx.compose.ui.Modifier
//import androidx.compose.ui.graphics.Color
import androidx.compose.ui.text.style.TextAlign
import androidx.compose.ui.tooling.preview.Preview
import androidx.compose.ui.unit.dp
import androidx.compose.ui.window.Popup
import androidx.compose.ui.window.PopupProperties
import com.example.microserveis.R
import com.example.microserveis.data.ColorToSelect


@Composable
fun ConfiguracioScreen(
    viewModel: MicroServeiViewModel = viewModel(),
    modifier: Modifier = Modifier,
    onAddNewHostConfiguration: (HostConfiguration) -> Unit = {},
    handleHostChange: (HostConfiguration) -> Unit = {}
) {
    val uiState by viewModel.uiState.collectAsState()

    var showDialog by remember { mutableStateOf(false) }

    var selectedIndex by rememberSaveable { mutableStateOf(0) }

    var colors = listOf<ColorToSelect>(
        ColorToSelect("Red", Color.Red),
        ColorToSelect("Green", Color.Green),
        ColorToSelect("Blue", Color.Blue),
        ColorToSelect("Yellow", Color.Yellow),
        ColorToSelect("Magenta", Color.Magenta),
        ColorToSelect("Cyan", Color.Cyan),
        ColorToSelect("White", Color.White),
        ColorToSelect("Black", Color.Black),
        ColorToSelect("Gray", Color.Gray)
    )

    Column {
        Button(
            onClick = { showDialog = true },
            modifier = Modifier
                .padding(16.dp)
        ) {
            Text("Add Host Configuration")
        }
        LazyColumn {
            items(uiState.hostConfigurations) { hostConfig ->
                HostConfigurationCard(hostConfig, handleHostChange)
            }
        }
        if (showDialog) {
            var name by remember { mutableStateOf("") }
            var color by remember { mutableStateOf(Color.White) }
            var host by remember { mutableStateOf("") }
            var port by remember { mutableStateOf("") }

            var selectedIndex by rememberSaveable { mutableStateOf(0) }

            AlertDialog(
                onDismissRequest = { showDialog = false },
                title = { Text("Add New Host Configuration") },
                text = {
                    Column {
                        OutlinedTextField(
                            value = name,
                            onValueChange = { name = it },
                            label = { Text("Name") }
                        )
                        // Color Picker (using a library like Accompanist or a custom implementation)
                        DropdownList(
                            itemList = colors,
                            selectedIndex = selectedIndex,
                            modifier = Modifier.padding(top = 8.dp),
                            onItemClick = { selectedId ->
                                selectedIndex = selectedId

                                color = colors[selectedId].color
                            }
                        )
                        OutlinedTextField(
                            value = host,
                            onValueChange = { host = it },
                            label = { Text("Host") }
                        )
                        OutlinedTextField(
                            value = port,
                            onValueChange = { port = it },
                            label = { Text("Port") }
                        )
                    }
                },
                confirmButton = {
                    Button(
                        onClick = {
                            onAddNewHostConfiguration(HostConfiguration(
                                name = name,
                                color = color.toArgb(),
                                host = host,
                                port = port
                            ))

                            showDialog = false
                        }
                    ) {
                        Text("Add")
                    }
                },
                dismissButton = {
                    Button(
                        onClick = { showDialog = false }
                    ) {
                        Text("Cancel")
                    }
                }
            )
        }
    }

}

@Composable
fun HostConfigurationCard(
    hostConfig: HostConfiguration,
    handleHostChange: (HostConfiguration) -> Unit = {}
) {
    Card(
        modifier = Modifier
            .fillMaxWidth()
            .padding(8.dp),
//            .clickable( // Add clickable modifier
//                indication = rememberRipple(), // Optional ripple effect
//                interactionSource = remember { MutableInteractionSource() }
//            ) { handleHostChange(hostConfig) },
        elevation = CardDefaults.cardElevation(defaultElevation = 2.dp),
        onClick = {handleHostChange(hostConfig)}
    ) {
        Row(
            modifier = Modifier
                .fillMaxWidth()
                .padding(16.dp),
            verticalAlignment = Alignment.CenterVertically
        ) {
            Box(
                modifier = Modifier
                    .size(48.dp)
                    .background(Color(hostConfig.color))
            )
            Spacer(modifier = Modifier.width(16.dp))
//            Row(
//                modifier = Modifier,
//                verticalAlignment = Alignment.CenterVertically,
//                horizontalArrangement = Arrangement.SpaceBetween
//            ) {
                Column {
                    Text(text = hostConfig.name, fontWeight = FontWeight.Bold)
                    Text(text = "Host: ${hostConfig.host}")
                    Text(text = "Port: ${hostConfig.port}")
                }
//                Button(onClick = { handleHostChange(hostConfig) }) {
//                    Text(text = "Swap")
//                }
//            }

        }
    }
}

@Composable
fun DropdownList(
    itemList: List<ColorToSelect>,
    selectedIndex: Int,
    modifier: Modifier,
    onItemClick: (Int) -> Unit
) {
    var showDropdown by rememberSaveable { mutableStateOf(false) }

    val scrollState = rememberScrollState()

    Column(
        modifier = Modifier,
        horizontalAlignment = Alignment.CenterHorizontally,
        verticalArrangement = Arrangement.Center) {
        // button
        Box(
            modifier = modifier
                .fillMaxWidth()
                .background(Color.LightGray)
                .clickable { showDropdown = true },
//                .clickable { showDropdown = !showDropdown },
            contentAlignment = Alignment.CenterStart
        ) {
            Row(
                modifier = Modifier
                    .padding(8.dp)
                    .fillMaxWidth(),
                verticalAlignment = Alignment.CenterVertically,
                horizontalArrangement = Arrangement.SpaceBetween
            ) {
                Text(text = itemList[selectedIndex].name, modifier = Modifier.padding(8.dp))
                Icon(
                    painter = painterResource(id = R.drawable.baseline_brush_24),
                    tint = if (itemList[selectedIndex].name == "White") Color.Black else Color.White,
                    contentDescription = "Color",
                    modifier = Modifier
                        .size(36.dp)
                        .background(
                            color = itemList[selectedIndex].color,
                            shape = CircleShape
                        )
                )
            }
        }
        Box() {
            if (showDropdown) {
                Popup(
                    alignment = Alignment.TopCenter,
                    properties = PopupProperties(
                        excludeFromSystemGesture = true,
                    ),
                    // to dismiss on click outside
                    onDismissRequest = { showDropdown = false }
                ) {

                    Column(
                        modifier = modifier
                            .heightIn(max = 90.dp)
                            .verticalScroll(state = scrollState)
                            .border(width = 1.dp, color = Color.Gray),
                        horizontalAlignment = Alignment.CenterHorizontally,
                    ) {

                        itemList.onEachIndexed { index, item ->
                            if (index != 0) {
                                HorizontalDivider(thickness = 1.dp, color = Color.LightGray)
                            }
                            Box(
                                modifier = Modifier
                                    .background(Color.Green)
                                    .fillMaxWidth()
                                    .clickable {
                                        onItemClick(index)
                                        showDropdown = !showDropdown
                                        println("Holi!")
                                    },
                                contentAlignment = Alignment.Center
                            ) {
                                Text(text = item.name)
                            }
                        }

                    }
                }


            }
        }
    }
}

@Preview
@Composable
fun HostConfigurationCardPreview() {
    HostConfiguration(name = "Test", color = Color.Red.toArgb(), host = "localhost", port = "8080")
}

@Preview
@Composable
fun DropdownListPreview() {
    DropdownList(
        itemList = listOf<ColorToSelect>(
            ColorToSelect("Red", Color.Red),
            ColorToSelect("Green", Color.Green),
            ColorToSelect("Blue", Color.Blue),
            ColorToSelect("Yellow", Color.Yellow),
            ColorToSelect("Magenta", Color.Magenta),
            ColorToSelect("Cyan", Color.Cyan),
            ColorToSelect("White", Color.White),
            ColorToSelect("Black", Color.Black),
            ColorToSelect("Gray", Color.Gray)
        ),
        selectedIndex = 0,
        modifier = Modifier,
        onItemClick = {})
}