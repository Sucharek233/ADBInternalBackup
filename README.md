# ADB Internal Storage Backup

This program lets you backup your internal storage using ADB.

### Why did I make this program?
I made it, because I didn't want to copy whole directories, and just look for what files I'm missing and let the program copy them. This saved me a lot of time while backing up my phone.

## Program walktrough, explanations, features

This is the main UI. Firstly, you have to connect a device.
![image](https://user-images.githubusercontent.com/31042508/206854659-5c057cb8-6e78-4426-bff7-b86f75b45e71.png)

You can connect with with USB or wirelessly. When connected, just select your device and click on Select.
![image](https://user-images.githubusercontent.com/31042508/206854688-1252aeea-4b78-4081-9f34-e6ffa2b20491.png)

Now you can start the scan. A regular one, or turbo.
This is how the scans work.

### **Regular scan:**
1. I look for all the folders in the phone. This is done by running `adb shell ls -R /sdcard`.
2. Then, I filter out all the files, so it's only folders
3. Now, I list the folders in output log and scan every single one. One by one. I know this is slow and inefficient.
4. While scanning the files, I check if it exists in the selected backup folder (more on that later).
5. Then, I display the whole output log and it's done.
6. The time to finish the scan can range from 5 to 20 minutes, depending how many files/folders you have on your phone.

**NOTES:**
1. When there are more than 50 files to list, output log will not list them during the scan. Everything will be displayed once the scan finishes.
2. You can turn on Faster scanning to turn off file listing while the scan is running. You can turn it off/on when the scan is running if you want to. It's recommended to turn it on, because it prevents crashes.

### **Turbo scan:**
1. I look for everything in the phone. This is also do by running `adb shell ls -R /sdcard`.
2. This time, I filter out all the folders with all of the files in them, making this process much, much faster.
3. The time to finish the scan can range from 5 to 40 seconds, depending how many files/folders you have on your phone.

**NOTES FOR ALL SCANS**

You might see one extra folder in scanning. No folder was in the selected folder. It could look like this:
![image](https://user-images.githubusercontent.com/31042508/206856592-35aaa40f-05e4-44ba-a8e3-0e1233bc2c4d.png)

This is because there's `/sdcard` in the scan as a folder, and I'm filtering it out. It's not filtering it out while counting the folders, so that's why there could be one extra.


Once the scan finishes, you can select a path where you want to backup the files to, and click on Copy missing/Backup.
![image](https://user-images.githubusercontent.com/31042508/206858299-f43a85a6-9bd8-4c43-9862-a8a189e4ebef.png)



If you have any questions/found bugs, please open an issue or message me on Discord at Sucharek#3516.
