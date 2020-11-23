# Test Latency via websocket

## Introduction

### Purpose

Measure round-trip time between two points over a websocket protocol.

This is for systems where only HTTP traffic is allowed through.

If you can use any protocol it is suggested that sockPerf is used instead of this solution.

#### test

connect ws://echo.websocket.org