package com.example.microserveis.communications

import com.example.microserveis.data.Servei
import com.example.microserveis.data.ServeiAAfectar
import com.google.gson.GsonBuilder
import okhttp3.OkHttpClient
import retrofit2.Retrofit
import retrofit2.converter.gson.GsonConverterFactory
import retrofit2.http.Body
import retrofit2.http.GET
import retrofit2.http.POST
import retrofit2.http.Url

object BaseUrlHolder {
    @Volatile var baseUrl: String = "http://10.0.2.2:3000"
}

val okHttpClient = OkHttpClient.Builder()
    .addInterceptor(BaseUrlInterceptor())
    .build()

private val retrofit = Retrofit.Builder()
    .baseUrl(BaseUrlHolder.baseUrl)
    .client(okHttpClient)
    .addConverterFactory(GsonConverterFactory.create(GsonBuilder().create()))
    .build()

interface StoreApiService {

    @GET("/services")
    suspend fun getServeis(): List<Servei>

    @POST("/changeServiceState")
    suspend fun canviarEstat(@Body id: ServeiAAfectar): List<Servei>

}

object ServeiApi {
    val retrofitService : StoreApiService by lazy {
        retrofit.create(StoreApiService::class.java)
    }
}