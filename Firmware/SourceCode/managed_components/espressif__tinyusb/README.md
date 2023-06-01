# Espressif TinyUSB fork

This is a fork of upstream [TinyUSB](https://github.com/hathach/tinyusb) with integration into ESP-IDF build system.
It is used mostly for rapid bugfixing and for releases independent from the upstream project.
We try to push all bugfixes and features to the upstream.

## How to use

There are two options of using TinyUSB with Espressif's SoCs.

### 1. Use this component together with [Espressif TinyUSB additions](https://github.com/espressif/idf-extra-components/tree/master/usb/esp_tinyusb/)

This is identical approach as in ESP-IDF 4.x releases. You can configure TinyUSB using Kconfig, as usual. Just add ``idf_component.yml`` to your main component with the following content::

```yaml
## IDF Component Manager Manifest File
dependencies:
  esp_tinyusb: "^1.0.0" # Automatically update minor releases
```

Or simply run:
```
idf.py add-dependency "esp_tinyusb^1.0.0"
```
### 2. Use TinyUSB only, without the [additions](https://github.com/espressif/idf-extra-components/tree/master/usb/esp_tinyusb/)

Use this option for custom TinyUSB applications.
In this case you will have to provide configuration header file ``tusb_config.h``. More information about TinyUSB configuration can be found [in official TinyUSB documentation](https://docs.tinyusb.org/en/latest/reference/getting_started.html).

You will also have to tell TinyUSB where to find the configuration file. This can be achieved by adding following CMake snippet to you main component's ``CMakeLists.txt``:

```cmake
idf_component_get_property(tusb_lib espressif__tinyusb COMPONENT_LIB)
target_include_directories(${tusb_lib} PRIVATE path_to_your_tusb_config)
```

Again, you can add this component to your project by adding ``idf_component.yml`` file:

```yaml
## IDF Component Manager Manifest File
dependencies:
  tinyusb: "~0.14.2" # Automatically update bugfix releases. TinyUSB does not guarantee backward compatibility
```

Or simply run:
```
idf.py add-dependency "tinyusb~0.14.2"
```

README from the upstream TinyUSB can be found in [hathach/tinyusb/README](https://github.com/hathach/tinyusb/blob/master/README.rst).
