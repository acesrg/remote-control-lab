# Developing from scratch

# Requirements

Quite simple, you will need: 
- make
- docker
- a machine with _amd64-like_ architecture, with some linux distro

# Walking into the containerized development environment

1. Clone this repo.
```bash
git clone https://gitlab.com/marcomiretti/remote-control-lab.git
```
2. Export your local wifi network credentials.
```bash
export WIFI_SSID=YOURSSID
export WIFI_PASS=YOURPASS
```
3. `cd` into your desired `target-system` and run the container.
```bash
cd <target-system>/esp
```
4. Run the container launcher
```bash
make dockeride
```
5. Start developing!

# Building and flashing

```bash
make
```

```bash
make flash
```

# Testing

```bash
make unittst
```
