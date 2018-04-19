# Smart-Alarm-Clock

**client_1.c** : Set a time and sent it to the server

**server_1.c** : Receive the setting time and compare to the current time

**passive_buzzer.c** : Play the music. When the touch sensor is touched over 3 seconds, stop playing the music

**server_withbuzzer.c** : Receive the setting time and compare to the current time, if the difference is <60s then beep.

----
**1st Solution to text2audio**

  sudo apt-get install espeak

**Then write a text.txt**

  espeak --stdout -s 80 text.txt > text.wav
  
  aplay text.wav
  
------
**2nd Solution to text2audio**
sudo apt-get install libttspico-utils

pico2wave -w lookdave.wav "Look Dave, I can see you're really upset about this." 

aplay lookdave.wav

-----

**3rd Solution to text2audio**

ttl_sample.c
-----

client_weather.c	Add files via upload	22 hours ago
demo.tar.gz	Add files via upload	22 hours ago
joystick_PS2.c	Add files via upload	a day ago
server_buzzer_joystick.c	Add files via upload	22 hours ago
tts_make.sh	tts_sample makefile and run	a day ago
tts_run.c	run tts_make.sh	a day ago
tts_sample.c	Add files via upload	2 days ago

-----
# LINK
http://www.angusfan.com/2017/03/16/Raspberry/
http://www.xfyun.cn/services/online_tts
