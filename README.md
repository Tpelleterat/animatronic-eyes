# animatronic-eyes

[![Animatronic eyes face detection](/docs/contents/video_thumbnail.png)](https://youtu.be/BoqDVRVQaFI)

## How to develop

To update code in Raspberry Pi, move folder files to card :

# Transfert code to server

```bash
scp -r Python eyes@192.168.1.51:/home/EyesPython
```

And run code :

```bash
python3 main.py
```

# Install service

Note : remplace user name by your default user

Tap command to grant usb access

```bash
sudo usermod -a -G tty pi
sudo usermod -a -G dialout pi
```

Install Systemd

```bash
sudo apt-get install -y systemd
```

Create service

```bash
sudo nano /etc/systemd/system/animatroniceyes.service
```

```bash
sudo systemctl daemon-reload
sudo systemctl enable animatroniceyes.service
sudo systemctl start animatroniceyes.service
```
