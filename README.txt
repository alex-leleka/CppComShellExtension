COM component, that extend system shell in Windows 7 and provide:

1.	Add menu item (Create Log) in Windows Explorer’s context menu (right click on the list of files).
2.	This menu item write information(file name, size, creation date, checksum) about selected files into the log file(FilesInfoShExtLog.log in the same directory as selected files).

External dependencies - Using threadpool, boost filesystem, boost thred, boost date_time.
