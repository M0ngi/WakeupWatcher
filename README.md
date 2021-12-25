# WakeupWatcher

Feel free to skip to the ***Guide*** section if you're not interested in the nerdy stuff. 

I wanted to document this experience for the sake of it as a first c++ project. So, here I'm.

### _Writeup_

So basically, I wanted to try to work on something in C++ & this was the first idea that came to my mind. This was a quick project & a good way to pass the time. I still have few ideas to implement in the future.

I've made the same thing in Python 3.7 but it felt as if it was too easy since there was a module for discord's API that took care of everything so I decided to actually try going to a lower level language.

After setting up Visual Studio & creating the project, I started working on the discord API as it'll be the main problem.
I had to look for a way to communicate with discord's web API so I decided to build my own functions & classes to do that.
I decided to use CPPRest in order to achieve that (which was helpful in parsing JSON too) & started working on the discord API objects.

Diving into discord's API documentation wasn't that easy at first but after a lot of googling, I was able to get started & send my "Hello world" message in a discord server.
Working on this project actually gave me the thought of coding my own C++ module for discord API, which will be the next thing I'll be working on.
After figuring out how to handle messages & channels, I had to figure out how to properly attach an image to a message. After the struggle & a lot of googling, 
I properly understood how it works.

Finally, when the discord API classes were ready, all I had left was creating pictures & sending them. Create 2 threads, one took care of the GPS Cords & the other took care of the
pictures, and voila, we are done.

This was a funny one & a great way to spend the time.


### _Guide_

* **What's the use of it?**

WakeupWatcher is an application that sends your computer's GPS cordinations to a discord server (which you specify & control) alongside pictures taken from your computer's camera.
The main goal of this application is to notify the owner of a computer of the use of their computers whenever it's not authorized, which would be helpful incase the device is stolen.
The application would wait for the user of the computer to write a safety password in a file (Both the file & the password are set by the owner) within a time period (set by the owner).
If the password is correct, the application would exit, else it'll start logging the GPS Cords + pictures to the discord server.

* **How to use?**

  1. Setup an autorun to the application.
  2. Edit the configuration file.
  
* **How to configure it?**

You need to create a discord server & a discord bot ( [Tutorial to do that](https://discordpy.readthedocs.io/en/stable/discord.html "Creating a bot account") ).
Copy your bot's token & set the `token` field equal to it in the `configModel.cfg` file (example of the line: `token=ABCDEFGHHJJJ` ).

Now, in your discord's server, create 4 channels: `log`, `img`, `error`, `gps` (you can change the names)

Now, via `discord.com/app` ( [Clickme](https://discord.com/app) ), navigate to your server and get your channels' IDs. When you open a channel via the webapp, the link will have the following format: `https://discord.com/channels/SERVER_ID/CHANNEL_ID`

We'll be interested in the channel_id part.

Now set the `err_ch`, `txt_ch`, `img_ch` & `log_ch` equal to the id of the channels you've created (`txt_ch` channel is used for the GPS).
An example of the 4 lines is the following:

```err_ch=919005199253782659
txt_ch=919005078151655424
log_ch=919005021536915546
img_ch=919005114948272159
```

Now, if you would like to set up a directory for the application to use to write the taken pictures in it, you can set it using the `pic_dir` path, note that the pictures are deleted right after sending.
You can keep it empty if you want to use the same directory as the application. (Example: `pic_dir=`)

Now, we'll be setting the safety file & the password. You can set the file using the `safe` line, example: `safe=C:/users/mongi/Desktop/New.txt`).
The `keyword` line is used to specify the password (Example: `keyword=mypass`).

Now, the time you'll have before the application starts sending information to the discord server is specified using the `count` line in seconds, example: `count=15` will wait for 15 seconds.

Finally, the `desktop_freq` is specified to set the frequency of sending screenshots, example: `desktop_freq=5` will send a screenshot every 5 camera pictures.
