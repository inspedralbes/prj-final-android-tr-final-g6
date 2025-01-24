package com.example.microserveis.communications

import okhttp3.HttpUrl
import okhttp3.HttpUrl.Companion.toHttpUrlOrNull
import okhttp3.Interceptor
import okhttp3.Response

class BaseUrlInterceptor: Interceptor {
    override fun intercept(chain: Interceptor.Chain): Response {
        var request = chain.request()
        val currentBaseUrl = BaseUrlHolder.baseUrl
        val newBaseHttpUrl = currentBaseUrl.toHttpUrlOrNull()

        // Verifica que la nova base URL sigui vàlida
        if (newBaseHttpUrl != null) {
            // Reemplaça la base URL de la petició original
            val newUrl = request.url
                .newBuilder()
                .scheme(newBaseHttpUrl.scheme)
                .host(newBaseHttpUrl.host)
                .port(newBaseHttpUrl.port)
                .build()

            // Crea una nova petició amb la nova URL
            request = request.newBuilder().url(newUrl).build()
        }

        return chain.proceed(request)
    }
}