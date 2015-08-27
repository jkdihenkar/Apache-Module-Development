#include<stdio.h>
#include<string.h>
#include<curl/curl.h>

int main() {

  //this example suppose that you have an echo server running that
  //logs the posted data on console
  //localhost/getData.php
  //find the sample server file getData.php

  char* postData="hello=hie";

  CURL *curl;

  curl = curl_easy_init();

  if(curl) {
    curl_easy_setopt(curl, CURLOPT_URL,"localhost:3000/getData.php");
    curl_easy_setopt(curl, CURLOPT_POST,1L);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS,postData);
    curl_easy_perform(curl);
    curl_easy_cleanup(curl);
  }
  return 0;
}
