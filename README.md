<a href="https://www.hardwario.com/"><img src="https://www.hardwario.com/ci/assets/hw-logo.svg" width="200" alt="HARDWARIO Logo" align="right"></a>

# Firmware for HARDWARIO Sigfox Climate Monitor

[![build](https://github.com/hardwario/twr-sigfox-climate-monitor/actions/workflows/main.yml/badge.svg)](https://github.com/hardwario/twr-sigfox-climate-monitor/actions/workflows/main.yml)
[![Release](https://img.shields.io/github/release/bigclownlabs/bcp-sigfox-climate-monitor.svg)](https://github.com/bigclownlabs/bcp-sigfox-climate-monitor/releases)
[![License](https://img.shields.io/github/license/bigclownlabs/bcp-sigfox-climate-monitor.svg)](https://github.com/bigclownlabs/bcp-sigfox-climate-monitor/blob/master/LICENSE)
[![Twitter](https://img.shields.io/twitter/follow/hardwario_en.svg?style=social&label=Follow)](https://twitter.com/hardwario_en)

![Photo of Sigfox Climate Monitor assembly](doc/sigfox-climate-monitor.png)

This repository contains firmware for Sigfox environmental sensor based on Climate Module.
Firmware is programmed into [Core Module](https://shop.bigclown.com/products/core-module).
Binary version is available in section [Releases](https://github.com/bigclownlabs/bcp-sigfox-climate-monitor/releases).

> Detailed information about this project can be found in [HARDWARIO Documentation](https://doc.bigclown.com).

**TODO** Link to documentation article

## Introduction

Sigfox Climate Station is a battery-operated indoor device integrating these sensors:

* **Thermometer** (ambient temperature)
* **Hygrometer** (relative air humidity)
* **Lux meter** (light intensity)
* **Barometer** (atmospheric pressure)

This device is able to run from two AAA Alkaline batteries for at least 1 year.
It reports measured data to Sigfox network every 30 minutes.
You can route sensor data as HTTP POST request with JSON body to your own web app via [MySigfox](https://www.mysigfox.com) service.

## Hardware

The following hardware components are used for this project:

* **[Climate Module](https://shop.bigclown.com/products/climate-module)**
* **[Core Module](https://shop.bigclown.com/products/core-module)**
* **[Sigfox Module](https://shop.bigclown.com/products/sigfox-module)**
* **[Mini Battery Module](https://shop.bigclown.com/products/mini-battery-module)**

## License

This project is licensed under the [MIT License](https://opensource.org/licenses/MIT/) - see the [LICENSE](LICENSE) file for details.

---

Made with &#x2764;&nbsp; by [**HARDWARIO a.s.**](https://www.hardwario.com/) in the heart of Europe.
