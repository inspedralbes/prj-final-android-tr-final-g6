package com.example.microserveis.data

import androidx.room.Dao
import androidx.room.Database
import androidx.room.Entity
import androidx.room.Insert
import androidx.room.PrimaryKey
import androidx.room.Query
import androidx.room.Room
import androidx.room.RoomDatabase

@Database(entities = [HostConfiguration::class], version = 1)
abstract class AppDatabase : RoomDatabase() {
    abstract fun hostConfigurationDao(): HostConfigurationDao
}

@Entity(tableName = "host_configurations")
data class HostConfiguration(
    @PrimaryKey(autoGenerate = true) val id: Int = 0, // Auto-generated ID
    val name: String,
    val color: Int, // Store color as an Int (Color.toArgb())
    val host: String,
    val port: String
)

@Dao
interface HostConfigurationDao {
    @Insert
    suspend fun insert(hostConfiguration: HostConfiguration)

    @Query("SELECT * FROM host_configurations WHERE id = :id")
    suspend fun getById(id: Int): HostConfiguration

    @Query("SELECT * FROM host_configurations")
    suspend fun getAll(): List<HostConfiguration>

    @Query("DELETE FROM host_configurations")
    suspend fun deleteAll()
}

