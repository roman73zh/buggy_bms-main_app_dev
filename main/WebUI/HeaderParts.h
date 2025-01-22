#pragma once

const char P_header_1[] = R"=====(<!DOCTYPE html>
<html lang="en">
  <head>
    <meta charset="utf-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <link rel="icon" href="../../favicon.ico">
    <title>)=====";

// title of the page

const char P_header_2[] =  R"=====(</title>
    <link href="bootstrap.min.css" rel="stylesheet">
    <link href="theme.css" rel="stylesheet">
    <script type = "text/javascript" src = "/script.js"></script>
    <link rel="shortcut icon" type="image/x-icon" href="/favicon.ico">
  </head>

  <body onload = "load_stats();">
    <nav class="navbar navbar-inverse navbar-fixed-top">
      <div class="container">
        <div class="navbar-header">
          <button type="button" class="navbar-toggle collapsed" data-toggle="collapse" data-target="#navbar" aria-expanded="false" aria-controls="navbar">
            <span class="sr-only">Toggle navigation</span>
            <span class="icon-bar"></span>
            <span class="icon-bar"></span>
            <span class="icon-bar"></span>
          </button>
          <a class="navbar-brand" href="#">)=====";

// unit name

const char P_header_3[] =  R"=====(</a>
        </div>
        <div id="navbar" class="navbar-collapse collapse">
          <ul class="nav navbar-nav">)=====";

// links in navbar

const char P_header_4[] =  R"=====(
          </ul>
        </div>
      </div>
    </nav>
  )=====";