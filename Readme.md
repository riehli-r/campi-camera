CamPi caméra
============


### Dependencies
Campi-camera needs libjpeg-dev.
```
sudo apt-get install libjpeg-dev
```

### Compilation
Use make to compile the project
```
make
```
### Install
Open the webcam.service file with your favorite editor (vim) and replace User and Group by yours.
```
User=pi
Group=pi
```
Then, you just have to install as root
```
sudo make install<br/>
```


-------

## Protocol

Ask server for a new camera id
```
req#id
```

Reconnect to the server with an old id
```
reco#{ID}
```
