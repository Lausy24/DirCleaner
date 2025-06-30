#include <iostream> //iostream für console out/input
#include <filesystem> //neue c++ 17 lib für file manipulation
#include <unordered_map> //hashmap für schnelles nachschlagen
#include <string> //strings wegen text

using namespace std; //damit man nicht immer std:: machen muss bzw für die einfachkeit des projekts
namespace fs = std::filesystem; //std::filesystem abkürzen zu nur fs



unordered_map<string, string> extensionsToFolder = {
    /* die hashmap, um einfacher nachzuschlagen, welcher ordner für welche extension genutzt wird.
     * schlüssel und wert definirt, bzw dateiendung und ordnername */

    {".jpeg","Pictures"},
    {".png","Pictures"},
    {".gif","Pictures"},
    {".tif","Pictures"},

    {".mp4","Videos"},
    {".mov","Videos"},
    {".avi","Videos"},
    {".mkv","Videos"},
    {".webm","Videos"},

    {".pdf", "Documents"},
    {".docx", "Documents"},
    {".txt", "Documents"},
    {".xls", "Documents"},
};

void startingText() { //für verbesserungvorschläge
    cout << "Made by Lausy24 on github\n";
    cout << "https://github.com/Lausy24\n";
    cout << "If you have any suggestions for more file extensions write me on the email on myportfolio\n";
    cout << "https://gracious-tribute-351783.framer.app\n";
}

/*  jetzt kommt die funktion moveFileToFolder, mit den parametern:
 *  filepath für den aktuellen Dateipfad
 *  baseDir für den aufzuräumenden ordner
 *  folderName für den zielordnername, z.b Documents */

void moveFileToFolder(const fs::path& filePath, const fs::path& baseDir, const string& folderName) {
    fs::path targetDir = baseDir / folderName; //neuer pfad targetDir wird erstellt (zielordner), mit / wird foldername drangehängt
    if (!fs::exists(targetDir)) { //wenn tragetDir nicht existiert (dass ! bedeuted nicht), dann erstellt er targetDir.
        fs::create_directory(targetDir);
    }
    fs::path targetPath = targetDir / filePath.filename(); //neuer pfad targetPath wird als targetDir ertsellt und angehöngt mit dateinamne plus erweiterung

    // Folgendes ist die Schleife um nicht mit doppelten dateinamen zu crashen
    int count = 1; //counter initialisieren für gleiche filenames
    while (fs::exists(targetPath)) { //wenn gleicher filename existiert, schleife öffnen
        /* Jetzt wird die datei neu benannt.
         * filePath.stem() gibt den namen ohne erweiterung also statt bild.png einfach nur bild
         * filePath.extension() gibt ihm die erweiterung zurück
         * dazwischen befindet sich der counter, also bei der zweiten gleichen datei = bild (2).png */
        targetPath = targetDir / (filePath.stem().string() + " (" + to_string(count) + ")" + filePath.extension().string());
        count++; //dass gleiche wie count = count + 1;
    }

    fs::rename(filePath, targetPath); //hier wird die datei verschoben, rename bennent oder verschiebt eine datei, ähnlich wie mv in unix.
                                              //filePath ist die originaldatei, targetpath der neue standort
    cout << "Moved: " << filePath.filename() << " to " << folderName << "/" << targetPath.filename() << "\n"; //hier wird kurz in die konsole geprintet wohin alles gegnagen ist
}

void cleanFolder(const fs::path& folderPath) {
    if (!fs::exists(folderPath) || !fs::is_directory(folderPath)) { //dieses if prüft ob der pfad existiert und ob er ein ordner ist, sonst gibts fehermeldung
        cout << "Folder does not exist here.\n";
        return;
    }

    for (auto& entry : fs::directory_iterator(folderPath)) { //hier startet eine schleife die alle dateien und ordner durschläuft und ein entry gibt
                                                             //ein entry ist ein kleines infoblatt mit den wichtigsten daten zur datei z.b. grösse name typ pfad etc.
        if (entry.is_regular_file()) { //prüft ob in den entrys eine regular file gibt
            fs::path file = entry.path(); //wenn ja, dann wird der pfad in einer variable file gespeichert, entry.path() gibt dann den vollständigen pfad zurück
            string ext = file.extension().string(); //die datei erweiterung wird als string ausgelesen
            //wandelt gleich alle buchstaben der erweiteung als kleinuschstaben um (probleme vorzeitig vermeiden weil windows oft grosse extensions schreibt, linux kleine)
            for (auto& c : ext) c = tolower(c);

            if (extensionsToFolder.find(ext) != extensionsToFolder.end()) { //prüft ob die erweiterung in unserer hashmap extensionsToFolder definiert ist
                moveFileToFolder(file, folderPath, extensionsToFolder[ext]); //ruft die funktion moveFileToFolder auf die vorhin erklärt wurde
            }
         }
    }
}

int main() {
    startingText();
    fs::path currentDir = fs::current_path(); //ermittelt den aktuellen pfad mit fs::current_path() und speichert das in der variable currentDir
    cout << "Currently cleaning path: " << currentDir << "\n"; //printet den aktuellen Pfad in der Konsole
    cleanFolder(currentDir); //ruft die funktion von vorhin auf mit dem zielpfad in dem das programm gestartet wurde (currentDir)
    cout << "Finished!\n";  //meldung dass alles geklappt hat
    cout << "Press ENTER to close..."; //damit sich dass programm nicht direkt schliesst und man nichts lesen kann
    cin.get();
    return 0;
}
