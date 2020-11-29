# Test Latency via websocket

## Introduction

### Purpose

Measure round-trip time between two points over a websocket protocol.

This is for systems where only HTTP traffic is allowed through.

If you can use any protocol it is suggested that sockPerf is used instead of this solution.

#### test

connect ws://echo.websocket.org


#### finding something

async_write, is what iostream

m_output_stream

See: /websocketpp/transport/iostream/connection.hpp


Maybe I need the base to write my own connection layer?

Is it at the Client level that I have to re-write this for TCP direct?

Do I start with the config file: /websocketpp/config/minimal_server.hpp