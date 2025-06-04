
#include "FlashDataHelper.h"

void FlashDataHelperClass::downloadContentFiles(FileServiceClass &fileService) 
{
    WiFiClientSecure client;
    client.setInsecure();

    HTTPClient https;
    https.begin(client, "raw.githubusercontent.com", 443, "/stefunkk/openager/master/data/index.html");

    https.GET();
    String indexFile = https.getString();

    fileService.saveFile("/index.html", indexFile);
    fileService.saveFile("/data.csv", "");
}
