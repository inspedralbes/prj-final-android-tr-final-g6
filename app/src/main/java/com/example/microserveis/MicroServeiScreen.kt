package com.example.microserveis

import android.app.Application
import androidx.compose.foundation.layout.padding
import androidx.compose.material.icons.Icons
import androidx.compose.material.icons.automirrored.filled.ArrowBack
import androidx.compose.material.icons.filled.Settings
import androidx.compose.material3.ExperimentalMaterial3Api
import androidx.compose.material3.Icon
import androidx.compose.material3.IconButton
import androidx.compose.material3.MaterialTheme
import androidx.compose.material3.Scaffold
import androidx.compose.material3.Text
import androidx.compose.material3.TopAppBar
import androidx.compose.material3.TopAppBarDefaults
import androidx.compose.runtime.Composable
import androidx.compose.runtime.getValue
import androidx.compose.ui.Modifier
import androidx.compose.ui.platform.LocalContext
import androidx.compose.ui.res.painterResource
import androidx.compose.ui.res.stringResource
import androidx.lifecycle.ViewModelProvider
import androidx.lifecycle.viewModelScope
import com.example.microserveis.ui.MicroServeiViewModel
import androidx.lifecycle.viewmodel.compose.viewModel
import androidx.navigation.NavHostController
import androidx.navigation.compose.NavHost
import androidx.navigation.compose.composable
import androidx.navigation.compose.currentBackStackEntryAsState
import androidx.navigation.compose.rememberNavController
import com.example.microserveis.ui.ConfiguracioScreen
import com.example.microserveis.ui.LlistatScreen
import com.example.microserveis.ui.LogsScreen

enum class ServeiScreen {
    Llistat,
    Configuracio,
    Logs,
    ErrorLogs
}

@Composable
fun MicroServeiScreen(
    viewModel: MicroServeiViewModel = viewModel(
        factory = ViewModelProvider.AndroidViewModelFactory(LocalContext.current.applicationContext as Application)
    ),
    navController: NavHostController = rememberNavController()
){
    val backStackEntry by navController.currentBackStackEntryAsState()

    val currentScreen = ServeiScreen.valueOf(
        backStackEntry?.destination?.route ?: ServeiScreen.Llistat.name
    )

    Scaffold(
        modifier = Modifier,
        topBar = {
            MicroServeiAppBar(
                currentScreen = currentScreen,
                canNavigateBack = navController.previousBackStackEntry != null,
                navigateUp = { navController.popBackStack() },
                modifier = Modifier,
                onConfigClick = {
                    viewModel.getHostsFromDB()
                    navController.navigate(ServeiScreen.Configuracio.name)
                }
            )
        }
    ) { innerPadding ->

        NavHost(
            navController = navController,
            startDestination = ServeiScreen.Llistat.name,
            modifier = Modifier.padding(innerPadding)
        ) {
            composable(route = ServeiScreen.Llistat.name) {
                LlistatScreen(
                    modifier = Modifier
                        .padding(innerPadding),
                    serveisViewModel = viewModel,
                    ChangeState = {
                        viewModel.changeState(it)
                    },
                    goToLogs = {

                        viewModel.setLogging(it)
                        navController.navigate(ServeiScreen.Logs.name)
                    },
                    goToErrorLogs = {
                        viewModel.setLogging(it)
                        navController.navigate(ServeiScreen.ErrorLogs.name)
                    }
                )
            }
            composable(route = ServeiScreen.Logs.name) {
                LogsScreen(
                    modifier = Modifier
                        .padding(innerPadding),
                    logsViewModel = viewModel,
                    mode = "logs"
                )
            }
            composable(route = ServeiScreen.ErrorLogs.name) {
                LogsScreen(
                    modifier = Modifier
                        .padding(innerPadding),
                    logsViewModel = viewModel,
                    mode = "error"
                )
            }
            composable(route = ServeiScreen.Configuracio.name) {
                ConfiguracioScreen(
                    modifier = Modifier
                        .padding(innerPadding),
                    onAddNewHostConfiguration = {

                        viewModel.insertHost(it)
                    },
                    handleHostChange = {
                        viewModel.updateBaseUrl(it)
                        viewModel.getServeis()

                        navController.navigate(ServeiScreen.Llistat.name)
                    }
                )
            }
        }
    }
}

@OptIn(ExperimentalMaterial3Api::class)
@Composable
fun MicroServeiAppBar(
    currentScreen: ServeiScreen,
    canNavigateBack: Boolean,
    navigateUp: () -> Unit,
    onConfigClick: () -> Unit = {},
    modifier: Modifier = Modifier
) {
    TopAppBar(
        title = { Text(currentScreen.name) },
        colors = TopAppBarDefaults.mediumTopAppBarColors(
            containerColor = MaterialTheme.colorScheme.primaryContainer
        ),
        modifier = modifier,
        navigationIcon = {
            if (canNavigateBack) {
                IconButton(onClick = navigateUp) {
                    Icon(
                        imageVector = Icons.AutoMirrored.Filled.ArrowBack,
                        contentDescription = stringResource(R.string.back_button)
                    )
                }
            }
        },
        actions = {
            IconButton(onClick = { onConfigClick() }) {
                Icon(
                    imageVector = Icons.Filled.Settings,
                    contentDescription = "Configuració"
                )
            }
        }
    )
}