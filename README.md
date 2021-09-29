BaseWar
=======

BaseWar is a cocos2d-x game available on the Android and iOS app store.
https://play.google.com/store/apps/details?id=com.plkl&hl=en
https://itunes.apple.com/at/app/basewar/id709940184?mt=8&ign-mpt=uo%3D4


The repositiory contains:
* client --> Client implementation.
* server --> Quick and dirty server implementation. (database structure missing, see note below)


GIT version vs app store version:
The version available here is the app store version before taking down the server.
Thus, if you test the current version on the app store you will recognize following missing points:
- Multiplayer
- Feedback button in main menu


If there is any interest I will write about my experience with this project on my blog at http://plankes-projects.weebly.com/
Just let me know if someone is interested.


NOTE:
Database structure got lost in time. If you want to use this project you have to either remove the server part from the client or reverse engineer the database structure. Please create a pull request in case you fix this project.
