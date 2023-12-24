# Arduino
## Team 03 - RoomSense

Team members responsible: Roman Gordon, Boldizsar Olajos
Reviewers: Abel Turlej, Anna Vorozhtsova, Jasper Marichal

### Installation and Configuration Instructions
#### Install device
1. Mount the device to a wall or a place you want
2. Plug the device into a power outlet
3. Connect the device to your computer to connect to the wifi network
4. Once the device is connected to the wifi network you will have wireless connectivity

#### Configure Device to WiFi network
##### Start Configuration
1. Connect to the device via the USB-B port
2. Open a serial monitor software
3. Enter the command “SENDING OFF” to pause the sending of the data
4. Then enter either “CONFIG EDIT” or “CONFIG RESET” to enter configuration mode

##### Possible Commands
###### Basic Configuration Commands
- Command “SSID <network name>”: Change the SSID that the WiFi connects to
- Command “PASS <wifi password>”: Change the password that the WiFi connects to
- Command “USER <username>”: The username and identity for a WPA2-Enterprise network (experimental feature)

###### Static IP Configuration Commands
*Addresses and Subnet Masks must be entered in octet form*
- Command “IP <ip address>”: Set the static IP address to connect to
- Command “SN <subnet mask>”: Set the subnet mask of the local network
- Command “GW <gateway address>”: Set the gateway address of the local network
- Command “PORT <port number>”: Change the port to start the server at

###### Save Configuration
1. Enter the command “CONFIG COMMIT” to save the configuration changes
1. Or enter the command “CONFIG RESTORE” to discard the changes (only restore correctly if “CONFIG EDIT” was used)
2. Then unplug the device from the computer and plug it back in to its own USB-A port
3. Plug the device into the power outlet using the DC adapter

##### General Commands
- Command “CONFIG SHOW”: Show the active configuration
- Command “STATUS”: Show the wifi adapter status
