<a href="https://www.bigclown.com"><img src="https://s3.eu-central-1.amazonaws.com/bigclown/gh-readme-logo.png" alt="BigClown Logo" align="right"></a>

# Firmware for BigClown Sigfox Climate Monitor

[![Travis](https://img.shields.io/travis/bigclownlabs/bcp-sigfox-climate-monitor/master.svg)](https://travis-ci.org/bigclownlabs/bcp-sigfox-climate-monitor)
[![Release](https://img.shields.io/github/release/bigclownlabs/bcp-sigfox-climate-monitor.svg)](https://github.com/bigclownlabs/bcp-sigfox-climate-monitor/releases)
[![License](https://img.shields.io/github/license/bigclownlabs/bcp-sigfox-climate-monitor.svg)](https://github.com/bigclownlabs/bcp-sigfox-climate-monitor/blob/master/LICENSE)
[![Twitter](https://img.shields.io/twitter/follow/BigClownLabs.svg?style=social&label=Follow)](https://twitter.com/BigClownLabs)

![Photo of Sigfox Climate Monitor assembly](doc/sigfox-climate-monitor.png)

This repository contains firmware for Sigfox environmental sensor based on Climate Module.
Firmware is programmed into [Core Module](https://shop.bigclown.com/products/core-module).
Binary version is available in section [Releases](https://github.com/bigclownlabs/bcp-sigfox-climate-monitor/releases).

> Detailed information about this project can be found in [BigClown Documentation](https://doc.bigclown.com).

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

Made with &#x2764;&nbsp; by [BigClown Labs s.r.o.](https://www.bigclown.com) in Czech Republic.
