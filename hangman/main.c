#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <time.h>
#include <ctype.h>

#define MAX_WORDS 5000
#define MAX_LENGTH 50
#define MAX_TRIES 10

//For Language Support. Right now German and English
//Language packs in json recommended, when using more languages
typedef enum { DE, EN } Language;


//Set Color
void set_color(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

//Drawing a little ASCII-Hangman
void printHangman(int tries) {
    printf("\n");
    set_color(6);
    const char *hangman[] = {
        " \n \n \n \n \n", //Print Only Whitespace
        " \n \n \n \n \n_____\n", //Base
        " |\n |\n |\n |\n |\n_|____\n", //Mast
        " _______\n |/\n |\n |\n |\n |\n_|____\n", //Beam
        " _______\n |/    |\n |\n |\n |\n |\n_|____\n", //Rope
        " _______\n |/    |\n |     O\n |\n |\n |\n_|____\n", //Head
        " _______\n |/    |\n |     O\n |     |\n |\n |\n_|____\n", //Body
        " _______\n |/    |\n |     O\n |     |\n |    / \n |\n_|____\n", //Left Leg
        " _______\n |/    |\n |     O\n |     |\n |    / \\\n |\n_|____\n", //Right Leg
        " _______\n |/    |\n |     O\n |    /|\n |    / \\\n |\n_|____\n", //Left Arm
        " _______\n |/    |\n |     O\n |    /|\\\n |    / \\\n |\n_|____\n" //Right Arm
    };
    printf("%s\n", hangman[tries]);
    set_color(7);
    printf("\n");
}

//Open Words File
void loadWords(const char *filename, char words[MAX_WORDS][MAX_LENGTH], int *count) {
    //Check if the File exists and open it
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("An error occurred while opening this file: %s\n", filename);
        exit(1);
    }

    *count = 0;
    while (fgets(words[*count], MAX_LENGTH, file) && *count < MAX_WORDS) {
        words[*count][strcspn(words[*count], "\n")] = '\0'; // Remove the Whitespace
        (*count)++;
    }

    fclose(file);
}

void makeFiles() {
    FILE *german;
    FILE *english;
    FILE *highscore;

    //German List
    german = fopen("woerter.txt", "w");
    if (!german)
        printf("An error occurred while creating this file: %s\n", "woerter.txt");

    char *woerter[MAX_WORDS] = {"ABENDESSEN", "ABSCHLUSS", "ACKER", "ADLER", "AFFE", "AHORN", "AKKU", "AKTEN", "ALGEN", "ALPEN", "AMEISE", "AMPEL", "ANANAS", "ANDEN", "APFEL", "APOTHEKE", "ARA", "ARBEIT", "ARZT", "AST", "ASTEROID", "ASTRONAUT", "AUTO", "BACH", "BACKOFEN", "BADWANNE", "BAGGER", "BAKTERIE", "BALL", "BANANE", "BASILIKUM", "BATTERIE", "BAUM", "BAUSTEINE", "BECHER", "BERG", "BERUF", "BESEN", "BESTECK", "BETT", "BIENE", "BIENENSTOCK", "BILD", "BIRKENBAUM", "BIRNE", "BISON", "BLATT", "BLEISTIFT", "BLEISTIFTSPITZE", "BLITZ", "BLUETE", "BLUMENKOHL", "BLUMENTOPF", "BODEN", "BOHNEN", "BONBON", "BOOT", "BORDSTEIN", "BRILLE", "BROETCHEN", "BROKKOLI", "BRUECKE", "BRUNNEN", "BUCH", "BUCHE", "BUCHREGAL", "BUECHERREGAL", "BUEGELBRETT", "BUERO", "BUNTSTIFT", "BURGER", "BUS", "BUTTER", "CANYON", "CARPORT", "CHEF", "CHEFIN", "CHINCHILLA", "CHIPS", "COMPUTER", "DACH", "DECKE", "DELFIN", "DELTA", "DILL", "DONNER", "DROMEDAR", "DRUCKER", "DUSCHE", "EBENE", "ECHINACEA", "EICHE", "EIMER", "EIS", "EISBAHN", "EISCREME", "ELEFANT", "ENTE", "ERBSEN", "ERDBEBEN", "ERDE", "ERDRUTSCH", "ESSIG", "EULE", "FAHRRAD", "FALKE", "FAMILIE", "FARBEN", "FARNE", "FEDER", "FEDERMAPPE", "FELD", "FELS", "FENSTER", "FENSTERBANK", "FENSTERGLAS", "FERNSEHER", "FEUER", "FEUERWEHR", "FICHTE", "FIEBER", "FILZSTIFT", "FISCH", "FLASCHE", "FLEISCH", "FLIEDER", "FLIEGE", "FLUGZEUG", "FLUSS", "FORST", "FREUND", "FREUNDIN", "FROSCH", "FRUCHT", "FRUEHSTUECK", "FUCHS", "GABEL", "GAEMSEN", "GAENSEBLUEMCHEN", "GALAXIE", "GARAGE", "GARDINE", "GARTEN", "GARTENHAUS", "GARTENZAUN", "GAZELLE", "GEBIRGE", "GEMAELDE", "GEMUESE", "GEMUESEGARTEN", "GERBIL", "GEWAECHSHAUS", "GEWAESSER", "GEWITTER", "GIRAFFE", "GLAS", "GRAS", "GRILLE", "GURKE", "GURT", "HAGEL", "HALBINSEL", "HAMSTER", "HANDSCHUHE", "HANDTUCH", "HANDY", "HANGEMATTE", "HAUS", "HECKE", "HEFT", "HEMD", "HERD", "HEUSCHRECKE", "HIMALAYA", "HIRSCH", "HOLUNDER", "HONIG", "HONIGMELONE", "HOSE", "HUEGEL", "HUND", "HUSTEN", "HYAENE", "IBIS", "IGEL", "IMPFUNG", "INSEL", "JACKE", "JAGUAR", "KABEL", "KAEFER", "KAFFEE", "KAKTUS", "KALENDER", "KAMEL", "KAMERA", "KAMILLE", "KANINCHEN", "KANU", "KAROTTE", "KARPATEN", "KARTOFFEL", "KATZE", "KEKS", "KERZE", "KIEFER", "KIRSCHE", "KISSEN", "KLASSENZIMMER", "KLEBER", "KLEBERFLASCHE", "KLEIDERSCHRANK", "KLEMMBRETT", "KLETTERGERUEST", "KLIPPE", "KNETE", "KNOBLAUCH", "KNOPF", "KNOSPE", "KOALA", "KOCH", "KOECHIN", "KOKOSNUSS", "KOLIBRI", "KOLLEGE", "KOLLEGIN", "KOMET", "KOPFHOERER", "KORALLE", "KRABBE", "KRAN", "KRANKENHAUS", "KRANKENWAGEN", "KRATER", "KREBS", "KREIDE", "KREISEL", "KREUZUNG", "KROKODIL", "KUCHEN", "KUECHE", "KUEHLSCHRANK", "KUERBIS", "KUESTE", "KUH", "KURS", "LABOR", "LADEGERAET", "LAERCHE", "LAMPE", "LASTWAGEN", "LAUBE", "LAUCH", "LAUTSPRECHER", "LAVA", "LAVENDEL", "LAWINE", "LEHRER", "LEHRERIN", "LEINWAND", "LEMUR", "LIBELLE", "LICHT", "LILIE", "LINEAL", "LINEALSKALA", "LINSEN", "LOEFFEL", "LOEWE", "LUCHS", "LUFT", "MAGMA", "MALBUCH", "MANGO", "MANGUSTE", "MANTEL", "MARDER", "MARMELADE", "MATRATZE", "MAUER", "MAULWURF", "MAUS", "MEDIKAMENT", "MEERSCHWEINCHEN", "MESSER", "METEOR", "MIKROFON", "MIKROSKOP", "MILCH", "MINZE", "MITTAGESSEN", "MOEWE", "MOND", "MONITOR", "MOOS", "MOTORRAD", "MUENDUNG", "MUETZE", "MURMELTIER", "MUSCHEL", "NACHTFALTER", "NASHORN", "NEBEL", "NEKTAR", "NETZ", "NOTIZBUCH", "NUDELN", "OBSERVATORIUM", "OBST", "OBSTBAUM", "OCTOPUS", "OEL", "OLIVENBAUM", "ORANGE", "ORDNER", "PALME", "PANDA", "PAPAGEI", "PAPAYA", "PARK", "PAUSE", "PAUSENHOF", "PAVILLON", "PETERSILIE", "PFANNE", "PFEFFER", "PFERD", "PFIRSICH", "PFLASTER", "PFLAUME", "PINSEL", "PIZZA", "PLANET", "PLANSCHBECKEN", "PLATEAU", "POLARREGION", "POLIZEI", "POLLEN", "POOL", "POSTER", "PRUEFUNG", "PULLOVER", "PUMA", "PUZZLE", "PYRENAEEN", "QUALLE", "QUELLE", "RADIERGUMMI", "RADIERGUMMISTUECK", "RADIESCHEN", "RAHMEN", "RAKETE", "RASEN", "RATTE", "RAUMSCHIFF", "RAUMSTATION", "REGAL", "REGEN", "REGENSCHIRM", "REGENWALD", "REGENWURM", "REH", "REIHER", "REIS", "REISSVERSCHLUSS", "REZEPT", "RINDE", "ROBBE", "ROBOTER", "ROCK", "ROCKIES", "ROLLER", "ROLLO", "ROSE", "ROSMARIN", "ROTEBEETE", "RUCKSACK", "RUTSCHE", "SAFT", "SALAT", "SALZ", "SAND", "SANDKASTEN", "SATELLIT", "SAVANNE", "SCANNER", "SCHAF", "SCHAL", "SCHALTER", "SCHAUFEL", "SCHAUKEL", "SCHERE", "SCHERENBLATT", "SCHIFF", "SCHILD", "SCHILDKROETE", "SCHLITTEN", "SCHLITTSCHUH", "SCHLOSS", "SCHLOSSTOR", "SCHLUCHT", "SCHLUESSEL", "SCHLUESSELBUND", "SCHMETTERLING", "SCHMETTERLINGSRAUPE", "SCHNECKE", "SCHNEE", "SCHNEEBALL", "SCHNEELEOPARD", "SCHNEEMANN", "SCHNUPFEN", "SCHOKOLADE", "SCHREIBBLOCK", "SCHREIBTISCH", "SCHUELER", "SCHUHE", "SCHULBUCH", "SCHULE", "SCHWAN", "SCHWIMMBECKEN", "SEE", "SEESTERN", "SEIFE", "SELLERIE", "SERVIETTE", "SESSEL", "SKI", "SKIZZE", "SNACK", "SNOWBOARD", "SOFA", "SONNE", "SONNENBLUME", "SONNENBRILLE", "SPECHT", "SPIEGEL", "SPIEGELSCHRANK", "SPIELPLATZ", "SPIELZEUG", "SPINAT", "SPINNE", "SPINNENNETZ", "SPORT", "SPRINGBRUNNEN", "STAFFELEI", "STAMM", "STAUBSAUGER", "STECKDOSE", "STEHLAMPE", "STEIN", "STEINBOCK", "STEPPE", "STERN", "STIEFEL", "STIFT", "STIFTHALTER", "STORCH", "STRAND", "STRASSE", "STROM", "STUDENT", "STUDENTIN", "STUHL", "STUHLBEIN", "STURM", "SUPPE", "T-SHIRT", "TABLET", "TAFEL", "TAL", "TANNE", "TASCHE", "TASCHENREISSVERSCHLUSS", "TASSE", "TASTATUR", "TAU", "TAUBE", "TAXI", "TEE", "TEICH", "TELESKOP", "TELLER", "TEPPICH", "TEXTMARKER", "THERMOMETER", "THYMIAN", "TIGER", "TINTE", "TINTENFISCH", "TINTENROLLER", "TISCH", "TISCHDECKE", "TOILETTE", "TOMATE", "TOPF", "TOR", "TORTE", "TRAKTOR", "TRAUBE", "TRIEB", "TROPEN", "TSUNAMI", "TUKAN", "TUNNEL", "TUR", "TURGRIFF", "TURNHALLE", "U-BOOT", "UHR", "UNIVERSITAET", "UNIVERSUM", "URWALD", "VASE", "VERBAND", "VERKEHR", "VIRUS", "VOGEL", "VORHANG", "VULKAN", "WAL", "WALD", "WAND", "WASCHMASCHINE", "WASSER", "WASSERFARBE", "WASSERMELONE", "WASSERRUTSCHE", "WECKER", "WEINREBE", "WELTRAUM", "WIESE", "WILDSCHWEIN", "WIND", "WISCHMOP", "WOLF", "WOLKE", "WUEHLMAEUSE", "WUERMER", "WUESTE", "WUESTENFUCHS", "WUESTENMAUS", "WURZEL", "YAK", "ZAHNBUERSTE", "ZAHNPASTA", "ZAUN", "ZEBRA", "ZEBRASTREIFEN", "ZEICHNUNG", "ZEITSCHRIFT", "ZEITSCHRIFTENSTAPEL", "ZEUGNIS", "ZITRONE", "ZITRONENBAUM", "ZUCCHINI", "ZUCKER", "ZUG", "ZWEIG", "ZWIEBEL", "ZYPRESSE", "AUSBILDUNG", "ABEND", "ABKUERZUNG", "ABKUERZUNGEN", "AG", "ALBUM", "ALTER", "AMT", "ANAL", "ANGEBOT", "ANGEBOTE", "ANZEIGE", "ANZEIGEN", "ARBEIT", "ART", "ARTIKEL", "AUFGABEN", "AUGEN", "AUGUST", "AUSWAHL", "BACHELOR", "BAND", "BASEL", "BAU", "BEGINN", "BERLIN", "BERN", "BESTAND", "BESUCH", "BESUCHER", "BETRIEB", "BEVOELKERUNG", "BEZEICHNUNG", "BILD", "BILDER", "BILDUNG", "BLICK", "BUCH", "CHF", "CLUB", "COOKIES", "DAMEN", "DANK", "DATEN", "DATES", "DE", "DESIGN", "DEUTSCH", "DEUTSCHE", "DEUTSCHEN", "DEUTSCHLAND", "DEZEMBER", "DICH", "DIE", "DILDO", "DOLLAR", "DORF", "DR", "EINHEIT", "EINSATZ", "EINWOHNER", "ELEMENTSYMBOL", "ELTERN", "ENDE", "ENERGIE", "ENTWICKLUNG", "ERFAHRUNG", "ERFAHRUNGEN", "ERFOLG", "ERGEBNISSE", "EROTIK", "EROTIKANZEIGEN", "ESCORT", "EU", "EURO", "EUROPA", "FALL", "FAMILIE", "FEBRUAR", "FETISCH", "FILM", "FLAECHE", "FOLGE", "FORM", "FORUM", "FRAGE", "FRAGEN", "FRANKEN", "FRANKFURT", "FRAU", "FRAUEN", "FREUNDIN", "FOERDERUNG", "GAY", "GEBIET", "GEBAEUDE", "GELD", "GEMEINDE", "GESCHICHTE", "GESCHICHTEN", "GESELLSCHAFT", "GMBH", "GOOGLE", "GRAD", "GRUND", "GRUPPE", "GAESTE", "HAMBURG", "HAND", "HAUS", "HAUT", "HILFE", "HOTEL", "HAELFTE", "HOEHE", "INFORMATIONEN", "INTERESSE", "INTERNET", "JAHR", "JAHRE", "JAHREN", "JAHRES", "JANUAR", "JULI", "JUNI", "KANTON", "KARRIERE", "KILOMETER", "KIND", "KINDER", "KINO", "KIRCHE", "KLEINANZEIGEN", "KOMMISSION", "KONTAKT", "KONTAKTANZEIGEN", "KONTAKTE", "KOSTEN", "KRIEG", "KUNDEN", "KUNST", "KOELN", "KOENIG", "KOERPER", "LAND", "LEBEN", "LEISTUNGEN", "LEUTE", "LIEBE", "LINZ", "LUST", "LUZERN", "LAENGE", "LOESUNG", "MAI", "MAL", "MANN", "MANNSCHAFT", "MARKT", "MASSAGE", "MASSNAHMEN", "MEDIEN", "MENSCHEN", "METER", "MILLIONEN", "MINUTEN", "MITARBEITER", "MITGLIED", "MITGLIEDER", "MITTE", "MUSCHI", "MUSIK", "MUTTER", "MAEDCHEN", "MAENNER", "MAERZ", "MOEGLICHKEIT", "MOEGLICHKEITEN", "MUENCHEN", "NAME", "NAMEN", "NATUR", "NEW", "NOVEMBER", "NUTZER", "NUTZUNG", "OKTOBER", "ORGASMUS", "ORT", "PAAR", "PAARE", "PARTNER", "PARTY", "PATIENTEN", "PENIS", "PERSON", "PERSONEN", "PLATZ", "POLITIK", "POLIZEI", "PORNO", "PORNOKINO", "PORNOS", "PRAXIS", "PREIS", "PREISE", "PROBLEM", "PROBLEME", "PRODUKTE", "PROGRAMM", "PROJEKT", "PROJEKTE", "PROZENT", "QUALITAET", "RAHMEN", "RAUM", "RECHT", "REGEL", "REGIERUNG", "REGION", "REIHE", "RICHTUNG", "ROLLE", "SAISON", "SALZBURG", "SAUNA", "SCHULE", "SCHULEN", "SCHWANZ", "SCHWEIZ", "SCHWEIZER", "SCHUELER", "SCHUELERINNEN", "SEEMEILE", "SEITE", "SEITEN", "SEPTEMBER", "SERVICE", "SICHERHEIT", "SOHN", "SOMMER", "SPIEL", "SPIELE", "SPIELER", "STADT", "STAND", "STEIERMARK", "STELLE", "STRASSE", "STRECKE", "STUDIUM", "STUNDEN", "SUCHE", "SWINGER", "SWINGERCLUB", "TAG", "TAGE", "TANTRA", "TEAM", "TEIL", "TEILNEHMER", "THEMA", "THEMEN", "TITEL", "TOCHTER", "TOD", "TREFFEN", "UHR", "UMSETZUNG", "UNIVERSITAET", "UNTERNEHMEN", "UNTERSTUETZUNG", "USA", "VATER", "VERANSTALTUNG", "VEREIN", "VERFUEGUNG", "VERGLEICH", "VERLAG", "VIDEO", "VIDEOS", "WEBSITE", "WEG", "WEISE", "WELT", "WERK", "WIEN", "WISSEN", "WOCHE", "WOCHEN", "ZAHL", "ZIEL", "ZUKUNFT", "ZUSAMMENARBEIT", "ZUERICH", "AUFGABE", "ZEITPUNKT", "TEILE", "BROWSER", "PUNKTE", "ANFORDERUNGEN", "BURG", "NORDEN", "FRANKREICH", "BAD", "BEITRAG", "SITUATION", "WISSEN", "ARTEN", "LAGE", "TIPPS", "UNION", "SERIE", "KOPF", "VERFAHREN", "GEMEINDEN", "HOCHSCHULE", "FIRMA", "JUGENDLICHEN", "VERANSTALTUNGEN", "ERGEBNIS", "FREUDE", "NAEHE", "AUSSTELLUNG", "WAHL", "BETT", "EHE", "ANZAHL", "INSEL", "BEHANDLUNG", "BERATUNG", "BRUDER", "SPERMA", "ENTSCHEIDUNG", "FORSCHUNG", "KUECHE", "LINIE", "ANTWORT", "SUEDEN", "LAENDERN", "MUND", "BONDAGE", "SYSTEM", "PARKPLATZ", "SPRACHE", "ANWENDUNG", "STAATEN", "DUESSELDORF", "SKLAVIN", "MASSAGEN", "ALLTAG", "ZENTRUM", "ARSCH", "FOTOS", "SCHUTZ", "FILME", "GESICHT", "SINNE", "WERT", "BASIS", "UMGANG", "AUTO", "FAMILIEN", "ESSEN", "ZIMMER", "PARTEI", "GESUNDHEIT", "ANSPRUCH", "IDEEN", "SCHLOSS", "HERR", "ENGAGEMENT", "BEREICHEN", "KLASSE", "LOESUNGEN", "MONATE", "NACHT", "STUECK", "PARLAMENT", "GRUNDLAGE", "MUSEUM", "WEBSEITE", "TECHNIK", "SOFTWARE", "FREUND", "BESITZ", "WELTKRIEG", "APP", "STAEDTE", "STIFTUNG", "RESTAURANT", "STUDIE", "MATERIAL", "OSTEN", "PRODUKT", "ANLAGE", "BAR", "KULTUR", "NACHRICHTEN", "REISE", "TRUPPEN", "LANDKREIS", "ABSCHLUSS", "JAENNER", "GANGBANG", "ANGABEN", "PROF.", "GEGENSETZ", "FREITAG", "VERBINDUNG", "SOLDATEN", "UMGEBUNG", "KRAFT", "BEZIEHUNG", "FAELLEN", "LITERATUR", "INDUSTRIE", "ANBIETER", "WOHNUNG", "DOMINA", "SICHT", "PRODUKTION", "ANTRAG", "BUNDESRAT", "SAMSTAG", "TIERE", "IDEE", "VERSION", "MEINUNG", "EXPERTEN", "EINRICHTUNGEN", "SCHIFF", "ZUG", "AUSLAND", "HUND", "ZEITSCHRIFT", "GIRLS", "MENSCH", "HERAUSFORDERUNGEN", "GRUPPEN", "BUERGER", "URLAUB", "BEWEGUNG", "GRUENDUNG", "BEGRIFF", "BAHNHOF", "STELLEN", "TURM", "ORGANISATION", "ZUSAMMENHANG", "DREIER", "FARBEN", "KOOPERATION", "KATEGORIE", "VIBRATOR", "VERTRAG", "PUBLIKUM", "VERWENDUNG", "LEISTUNG", "LIGA", "SPORT", "CLUBS", "KARL", "DAME", "MENGE", "KOMMUNIKATION", "TIROL", "SACHSEN", "BODEN", "DINGE", "FUNKTION", "DIGITALISIERUNG", "LANDES", "VIELFALT", "GROESSE", "STAFFEL", "LEIPZIG", "ZUGANG", "PORNOS", "SCHUELERINNEN", "INNSBRUCK", "EINFLUSS", "SACHE", "VERWALTUNG", "ARMEE", "UNTERRICHT", "DIENSTLEISTUNGEN", "KUENSTLER", "SHOP", "LEITUNG", "WINTER", "OBEROESTERREICH", "WERBUNG", "HERBST", "BEDUERFNISSE", "PARIS", "MILLIARDEN", "GARTEN", "ANGST", "ANLASS", "LADY", "HERSTELLER", "FRANZ", "RANG", "LICHT", "MONAT", "FOTO", "WIRKUNG", "KOLLEGEN", "MANNSCHAFTEN", "SONNTAG", "GOTT", "AUSTAUSCH", "WEITERBILDUNG", "FENSTER", "BEDARF", "YORK", "BEZIRK", "DARSTELLUNG", "POSITION", "GRENZEN", "TEILNAHME", "RENNEN", "TITTEN", "WAGEN", "VERANTWORTUNG", "BERICHT", "CHANCE", "KAMPF", "PRAESIDENT", "SMARTPHONE", "HERRSCHAFT", "GERAETE", "SITZ", "RUNDE", "INFOS", "AKTIE", "ANMELDUNG", "HERREN", "E-MAIL", "EINRICHTUNG", "LESBEN", "ZEITEN", "INTEGRATION", "FUEHRUNG", "WALD", "UMWELT", "GEDANKEN", "VORSITZENDER", "ARZT", "WUNSCH", "ANTEIL", "QUELLEN", "ZEITUNG", "VORAUSSETZUNGEN", "GLUECK", "KURS", "BEZUG", "FREUNDE", "DM", "AKTIVITAETEN", "PLANUNG", "SCHWERPUNKT", "STUNDE", "ANSCHLUSS", "CASINO", "FUNKTIONEN", "MOMENT", "WISSENSCHAFT", "GERAET", "BERUF", "SCHUELERN", "LONDON", "HANDEL", "GESPRAECH", "INSTITUT", "CHAT", "GELAENDE", "ITALIEN", "PFLEGE", "FAHRZEUGE", "KAERNTEN", "INHALT", "BUERGERMEISTER", "WEGE", "TISCH", "EUROPAS", "ABENTEUER", "PRAHA", "FARBE", "SEE", "ZIELE", "LEHRER", "ANZEIGEN", "BRUECKE", "ENGLISCH", "ENGLAND", "HOF", "SALZBURGER", "MOTTO", "STIL", "WUENSCHE", "AUFTRAG", "KONZEPT", "SCHLUSS", "UEBERBLICK", "AUSGABE", "GEMEINDERAT", "NUERNBERG", "KOMBINATION", "MITGLIEDSTAATEN", "KAPELLE", "LISTE", "SATZ", "POLIZEI", "PRUEFUNG", "SACHEN", "AUFLAGE", "BETREUUNG", "WORT", "VERLAUF", "FOKUS", "HOLZ", "STIMMEN", "UMFELD", "HAENDE", "MODELL", "NRW", "RECHNUNG", "MANAGEMENT", "WEIBCHEN", "LEITER", "MEISTER", "TUER", "ABSCHNITT", "BANK", "DRESDEN", "DRUCK", "SELBSTBEFRIEDIGUNG", "VATERS", "STANDORT", "ANLIEGEN", "FIRMEN", "DIENST", "STUDIERENDE", "START", "SAUNACLUB", "STRAND", "INITIATIVE", "EINFUEHRUNG", "HUNDE", "LINK", "PUNKT", "STUNDENHOTEL", "KOMMUNEN", "VERSORGUNG", "OEFFENTLICHKEIT", "HINTERGRUND", "BUEHNE", "LERNEN", "MITTEL", "SIEDLUNG", "OPFER", "OUTDOOR", "SAMMLUNG", "SWINGERCLUBS", "TORE", "FACEBOOK", "FLUGHAFEN", "VERKAUF", "ZENTIMETER", "DORTMUND", "FOLGEN", "HAEUSER", "PFLANZEN", "NETZWERK", "SUCHT", "FINALE", "NETZ", "SINN", "KRITIK", "MATERIALIEN", "ZEITRAUM", "STUDIO", "VIELZAHL", "AMATEUR", "BEWOHNER", "ZEICHEN", "EHEFRAU", "GESTALTUNG", "KLOSTER", "LEBENS", "CHANCEN", "DETAILS", "AKTION", "LIED", "MONTAG", "BUND", "SCHWESTER", "DISKUSSION", "ROMAN", "ZAHLEN", "BUNDESREGIERUNG", "TEAMS", "HERZ", "MITARBEITERN", "NACHFOLGER", "RISIKO", "GESCHMACK", "METHODEN", "PRODUKTEN", "KAUF", "FRUEHJAHR", "LANDWIRTSCHAFT", "RUECKKEHR", "HERAUSFORDERUNG", "FAHRT", "FINGER", "HEIMAT", "BRUESSEL", "EBENE", "PORTAL", "LUFT", "BEDINGUNGEN", "BEWERTUNG", "TEXT", "WORKSHOPS", "TAETER", "SCHIFFE", "GELEGENHEIT", "GRENZE", "KANTONE", "FRIEDHOF", "WOCHENENDE", "FEHLER", "UMSATZ", "DIENSTE", "MITTELPUNKT", "WETTER", "DONNERSTAG", "STUDIEN", "VORTEIL", "REGELN", "FAHRZEUG", "WETTBEWERB", "ATMOSPHAERE", "ENTSCHEID", "PLATTFORM", "NUTZEN", "BUECHER", "PROFIL", "MITTWOCH", "REGIONEN", "SINGLES", "KARTE", "INFORMATION", "SIEG", "HOCHSCHULEN", "STAAT", "UNTERNEHMENS", "SOEHNE", "THEATER", "BEINE", "MAENNCHEN", "WAPPEN", "MACHT", "PROFESSOR", "RAT", "GEWALT", "VEROEFFENTLICHUNG", "ZUSTAND", "VERHAELTNIS", "VORTEILE", "STRASSE", "BEHOERDEN", "ERDE", "BREITE", "SPITZE", "AUSWIRKUNGEN", "DURCHFUEHRUNG", "EINWOHNERN", "ORGANISATIONEN", "TEST", "DIENSTAG", "ERNAEHRUNG", "HINWEISE", "KOMMENTAR", "POLEN", "VORAUSSETZUNG", "LEIDENSCHAFT", "MEISTERSCHAFT", "GALLEN", "VERKEHR", "ANTWORTEN", "DURCHMESSER", "AERZTE", "LANDSCHAFT", "GROESSE", "VERTRETER", "VORTRAG", "URTEIL", "ADRESSE", "UNIVERSITAET", "WIEN", "HOTELS", "WINTERTHUR", "STRASSEN", "AUTOR", "BURGENLAND", "INTERESSEN", "PARTEIEN", "SONNE", "GUTSCHEIN", "CHINA", "STROM", "UNTERSUCHUNGEN", "ORTES", "BRANCHE", "VERAENDERUNGEN", "VERHALTEN", "JUGEND", "GEFAHR", "TRADITION", "FORMEN", "BEREICHE", "KONTROLLE", "KLASSEN", "SPIELZEIT", "ABTEILUNG", "JAPAN", "OSTRAVA", "KREIS", "PROZESS", "GAST", "RUHE", "VERGANGENHEIT", "HEKTAR", "KAISER", "MILLIMETER", "ANSPRECHPARTNER", "GENERATION", "NACHFRAGE", "FIGUR", "KUNDE", "WEIN", "SORTIMENT", "ANWENDUNGEN", "ERFAHRUNGEN", "REICH", "AUFBAU", "INFRASTRUKTUR", "ENERGIEN", "MORGEN", "GESCHAEFTSFUEHRER", "NACHRICHT", "SCHADEN", "RUECKEN", "TONNEN", "ANLAGEN", "GEMUESE", "TEXTE", "ZEITRAUM", "MODELL", "DIREKTOR", "FANS", "LEBENSMITTEL", "VERBESSERUNG", "HERRN", "TECHNOLOGIE", "EINWOHNERZAHL", "FIGUREN", "FOTOS", "GLAS", "ANALYSE", "BESTELLUNG", "RESSOURCEN", "GERICHT", "GATTUNG", "REGELUNGEN", "MINUTE", "GRIECHENLAND", "NIEDERSACHSEN", "KRANKENHAUS", "AENDERUNGEN", "PROJEKTEN", "THERAPIE", "VEREINS", "BRANDENBURG", "UNTERSUCHUNG", "DEMOKRATIE", "STIMME", "TOUR", "FERIEN", "MARKETING", "MARK", "QUELLE", "GRUENEN", "NIVEAU", "PROVINZ", "VERSUCH", "WORKSHOP", "FLUSS", "BETROFFENEN", "FAEHIGKEITEN", "HESSEN", "ENTSCHEIDUNGEN", "MITARBEITERINNEN", "STIMMUNG", "VERBRAUCHER", "ZUNGE", "TECHNOLOGIEN", "TELEFON", "STAEDTEN", "NEUBAU", "NACHMITTAG", "REISEN", "SZENE", "BESTANDTEIL", "EINBLICK", "TOURISMUS", "GEWERBE", "INVESTITIONEN", "HAARE", "TEILNEHMENDEN", "KLASSIKEN", "AUFWAND", "HERSTELLUNG", "UEBERSICHT", "RECHTE", "REPUBLIK", "STELLUNG", "NUMMER", "MEER", "STADT", "REIFE", "DAUER", "HOMEPAGE", "PROZESSE", "GRAD", "INSTITUTIONEN", "RESTAURANTS", "WUERDE", "KRISE", "LEHRE", "ARCHITEKTUR", "TRAINING", "KAFFEE", "UMFANG", "VERARBEITUNG", "SITZUNG", "PERSONAL", "UNTERLAGEN", "NUTZUNGSBEDINGUNGEN", "AUTOREN", "MITARBEITENDE", "TERMIN", "VORBEREITUNG", "STEIN", "KOMPETENZEN", "RUSSLAND", "ZUSCHAUER", "NACHHALTIGE", "AUFSTIEG", "GRUNDLAGEN", "KURSE", "PRESSE", "AUFSTELLUNG", "MAßE", "ZWEIFEL", "UNTERSCHIED", "BAUERN", "BUERO", "GEBURT", "TRAINER", "ASPEKTE", "VORGABEN", "AUFLOESUNG", "AUSBAU", "JOBS", "ABSATZ", "BEHINDERUNG", "BAHN", "BRUEDER", "WOHNUNGEN", "FLUCHT", "KLEIDUNG", "KOMMENTARE", "BESCHREIBUNG", "FLEISCH", "HANDWERK", "HERKUNFT", "BEZIEHUNGEN", "EINDRUCK", "BEWERBUNG", "PLAN", "DESSOUS", "VORSTAND", "GENF", "SPRACHEN", "SITUATIONEN", "GESCHICHTE", "ANFRAGE", "FEUERWEHR", "LEHRPERSONEN", "SPANIEN", "STUDENTEN", "SCHRIFTEN", "AUGSBURG", "CHOR", "REGENSBURG", "CARL", "AUSNAHME", "EINHEITEN", "ERWACHSENE", "SPITAL", "MAßNAHMEN", "UNTERSCHIED", "SAN", "BAYERN", "ASPEKTE", "BERUFLICHEN", "AUFSTIEG", "VORHABEN", "BEGLEITUNG", "CHARAKTER", "ELEMENTE", "ZWECK", "BESITZER", "HALLE", "FREIHEIT", "WICHTIG", "BABY", "AUFNAHME", "AENDERUNG", "DRITTEL", "HANDLUNG", "GEWICHT", "AUSSTATTUNG", "AFRIKA", "HAUSES", "HINWEIS", "ERKRANKUNGEN", "HAENDEN", "STRUKTUREN", "WEST", "ZAEHLER", "KIRCHEN", "AMBIENTE", "KATEGORIEN", "GRUENDE", "INNOVATION", "STELLUNGEN", "BRAND", "DATENSCHUTZ", "EIGENSCHAFTEN", "FREUNDEN", "ABLAUF", "BANKEN", "FRUEHSTUECK", "DOLMETSCHER", "FINANZIERUNG", "KENNTNISSE", "WIDERSTAND", "DATENSCHUTZERKLAERUNG", "BALL", "ERMITTLUNGEN", "GESCHAEFT", "THEORIE", "AUGE", "STATION", "BESCHAEFTIGTEN", "VORDERGRUND", "BLOG", "BUERGERINNEN", "SCHLACHT", "MUSIKER", "RADIO", "OBERFLAECHE", "WORTE", "AUSSICHT", "BILDERN", "INTERVIEW", "MEHRHEIT", "SCHWEDEN", "BIBLIOTHEK", "SPIELZEUG", "ANGABE", "MITTELALTER", "SACHSCHADEN", "METHODE", "AUFNAHMEN", "PRAESENTATION", "WACHSTUM", "WAFFEN", "AUFMERKSAMKEIT", "TRAUM", "FAHRER", "PHASE", "DACH", "GEGENWART", "EXEMPLARE", "MESSE", "FLAECHEN", "FELD", "MATHEMATIK", "MOBILITAET", "EREIGNISSE", "AGENTUR", "KRITERIEN", "PFARRER", "RESTE", "ANERKENNUNG", "SHOPS", "TOECHTER", "AUSDRUCK", "HOEHEPUNKT", "KARTEN", "BISCHOF", "FRUEHLING", "LERNENDEN", "VORSCHLAG", "STRUKTUR", "ZUGRIFF", "ABSTAND", "KAMERA", "RAEUME", "TREND", "BEISPIELE", "REST", "WEITERENTWICKLUNG", "ANALYSTEN", "ARBEITNEHMER", "EINGANG", "LAUFBAHN", "PROGRAMME", "ARZNEIMITTEL", "GAESTEN", "MASCHINEN", "POELTEN", "SEKUNDEN", "DANKE", "FORSCHER", "BENUTZER", "BESCHWERDE", "BUNDESAMT", "TAL", "DIVISION", "DOKUMENTE", "MASCHINE", "WERKEN", "FAKTOREN", "OESTERREICHER", "SCHWEIZERISCHE", "WANDEL", "ERKRANKUNG", "PAUSE", "RICHTER", "AUTOS", "PLAENE", "STAATSANWALTSCHAFT", "LEHRKRAEFTE", "EDELSTAHL", "FEUER", "GEBURTSTAG", "GEMAELDE", "PRAESIDENTEN", "ANREGUNGEN", "SCHWIERIGKEITEN", "AKTIEN", "DORFES", "VILLA", "VERBAND", "SCHAEDEN", "VERFASSUNG", "KOMFORT", "BRIEF", "LINIEN", "RAHMENBEDINGUNGEN", "TAGES", "MEDIZIN", "NEWSLETTER", "KLAEGERIN", "VEREINE", "ARCHITEKTEN", "BOCHUM", "FREIBURG", "ORTSTEIL", "KINDES", "SOLOTHURN", "GEBAEUDES", "LAGER", "SENDUNG", "ZUHAUSE", "AUFENTHALT", "GEFAENGNIS", "ORTE", "EROEFFNUNG", "LINZER", "MERKEL", "BETEILIGUNG", "AUSSAGE", "PAPIER", "SEELE", "ANFAENGER", "ANMELDUNG", "DOMAIN", "EVENT", "INSELN", "TAT", "GEMEINSCHAFT", "TRAEGER", "GRAF", "REGIERUNGSRAT", "STATUS", "ORTEN", "VORJAHR", "FEST", "SCHRITTE", "UNIVERSITAETEN", "MAINZ", "PARTNERN", "AUGSBURG", "ERKENNTNISSE", "INNENSTADT", "SCHULJAHR", "FALLE", "ERINNERUNG", "GEBIETEN", "FLUEGEL", "KREUZ", "BONN", "ERWEITERUNG", "ANLEITUNG", "STRASSEN", "ENTSPANNUNG", "BEDUERFNISSEN", "HERMANN", "KRANKHEITEN", "ULM", "DIALOG", "ERSTELLUNG", "PLAETZE", "SERVER", "BEWERTUNGEN", "EINLADUNG", "HAENDLER", "ENTSTEHUNG", "MUSEEN", "SAENGER", "BESCHWERDEN", "MUENSTER", "KONTO", "SCHAUSPIELER", "VORSTELLUNG", "VORFELD", "ANSATZ", "GOTTES", "GESPRAECHE", "GRAB", "KLIMA", "METALL", "OEL", "KONZERN", "AUSFUEHRUNG", "WAHRHEIT", "WECHSEL", "BEHOERDE", "GRUNDSCHULE", "ALPEN", "GEBUEHREN", "MOBILE", "FACHKRAEFTE", "GEGEND", "ORTSCHAFT", "GEIST", "VORSCHLAEGE", "BROT", "ERRICHTUNG", "REALITAET", "REDE", "MILCH", "SPEISEN", "ERGEBNISSEN", "FREIZEIT", "GENUSS", "KANTON", "BERN", "KELLER", "BACH", "BUS", "EINTRITT", "FAELLE", "STANDARD", "SCHNEE", "STADTTEIL", "FISCHER", "HOSE", "BLAETTER", "MUELLER", "THURGAU", "CDU", "KONZERT", "VEREINIGUNG", "ERZIEHUNG", "MANNHEIM", "VOLK", "SCHWANGERSCHAFT", "DUSCHE", "SCHUHE", "VARIANTE", "HAUPTSTADT", "TERMINE", "TURNIER", "VORLIEBEN", "UEBUNGEN", "VORLAGE", "WOHNEN", "AUSEINANDERSETZUNG", "RESULTATE", "BESATZUNG", "KLIMASCHUTZ", "STREIT", "ALKOHOL", "SALZ", "DEBATTE", "EINSTIEG", "SEMINAR", "TIEFE", "VIERTEL", "GEGENTEIL", "CENTER", "GEWINN", "SENIOREN", "UNGARN", "AKTEURE", "PERSPEKTIVEN", "POSTEN", "WALLIS", "KANADA", "ALBEN", "PARLAMENTS", "POLITIKER", "DENKEN", "HAFEN", "SPUREN", "UMBAU", "ZUCKER", "KINDERGARTEN", "SIEGER", "TERRASSE", "INTERNETSEITE", "PRAKTIKA", "SZENEN", "VOLKSZAEHLUNG", "AUSGABEN", "PARTNERIN", "VERNETZUNG", "KONZERTE", "HAUSHALT", "LESER", "RATHAUS"};
    for (int i = 0; woerter[i] !=  '\0'; i++) {
        fprintf(german, "%s\n", woerter[i]);
    }
    fclose(german);

    //English List
    english = fopen("words.txt", "w");
    if (!english)
        printf("An error occurred while creating this file: %s\n", "words.txt");

    char *words[MAX_WORDS] = {"CAT", "DOG", "MOUSE", "TABLE", "CHAIR", "LAMP", "FISH", "BIRD", "ELEPHANT", "GIRAFFE", "LION", "MONKEY", "HORSE", "ZEBRA", "TURTLE", "CROCODILE", "SHEEP", "COW", "DEER", "STAG", "RABBIT", "HAMSTER", "GUINEAPIG", "HEDGEHOG", "FROG", "CRAB", "SNAIL", "SPIDER", "BEE", "ANT", "FLY", "BUTTERFLY", "EAGLE", "FALCON", "OWL", "PARROT", "PIGEON", "DUCK", "DAISY", "ROSE", "LILY", "SUNFLOWER", "CACTUS", "TREE", "BRANCH", "LEAF", "ROOT", "STONE", "SAND", "WATER", "RIVER", "LAKE", "MOUNTAIN", "HILL", "CLOUD", "RAIN", "SNOW", "ICE", "FIRE", "AIR", "HOUSE", "WINDOW", "DOOR", "ROOF", "WALL", "FENCE", "KEY", "LOCK", "BOOK", "MAGAZINE", "PEN", "PENCIL", "ERASER", "RULER", "SCISSORS", "GLUE", "BAG", "BACKPACK", "CAMERA", "TELEVISION", "COMPUTER", "PHONE", "TABLET", "CLOCK", "MIRROR", "PICTURE", "POSTER", "CARPET", "VASE", "CANDLE", "BOTTLE", "CUP", "MUG", "SPOON", "KNIFE", "FORK", "POT", "PAN", "STOVE", "FRIDGE", "OVEN", "WASHINGMACHINE", "WHALE", "DOLPHIN", "SEAL", "STARFISH", "JELLYFISH", "OCTOPUS", "SQUID", "LOBSTER", "CLAM", "OYSTER", "CORAL", "SEAGULL", "SWAN", "HERON", "WOODPECKER", "KOALA", "PANDA", "RHINOCEROS", "BUFFALO", "GAZELLE", "HYENA", "JAGUAR", "COUGAR", "TIGER", "CAMEL", "DROMEDARY", "YAK", "LEMUR", "MONGOOSE", "FOX", "WOLF", "LYNX", "BADGER", "BOAR", "MOLE", "GERBIL", "CHINCHILLA", "RAT", "LEOPARD", "IBIS", "TOUCAN", "MACAW", "HUMMINGBIRD", "MOTH", "DRAGONFLY", "CRICKET", "GRASSHOPPER", "BEETLE", "WORM", "EARTHWORM", "CATERPILLAR", "WEB", "HIVE", "HONEY", "POLLEN", "NECTAR", "BLOSSOM", "BUD", "TRUNK", "BARK", "TWIG", "SHOOT", "MOSS", "FERN", "GRASS", "MEADOW", "FIELD", "VINE", "ORCHARD", "APPLE", "PEAR", "CHERRY", "PEACH", "PLUM", "GRAPE", "LEMON", "ORANGE", "BANANA", "PINEAPPLE", "MANGO", "PAPAYA", "COCONUT", "WATERMELON", "MELON", "CUCUMBER", "TOMATO", "CARROT", "POTATO", "ONION", "GARLIC", "LETTUCE", "SPINACH", "BROCCOLI", "CAULIFLOWER", "PEAS", "BEANS", "LENTILS", "PUMPKIN", "ZUCCHINI", "RADISH", "BEET", "CELERY", "LEEKS", "PARSLEY", "BASIL", "THYME", "ROSEMARY", "DILL", "MINT", "LAVENDER", "CHAMOMILE", "ELDERFLOWER", "LILAC", "BEECH", "OAK", "PINE", "SPRUCE", "FIR", "MAPLE", "LARCH", "CYPRESS", "PALM", "OLIVE", "BIRCH", "WIND", "STORM", "THUNDER", "LIGHTNING", "FOG", "DEW", "HAIL", "MOON", "SUN", "STAR", "PLANET", "GALAXY", "UNIVERSE", "EARTH", "SPACE", "SATELLITE", "ROCKET", "TELESCOPE", "OBSERVATORY", "ASTRONAUT", "SPACESHIP", "SPACESTATION", "COMET", "METEOR", "ASTEROID", "CRATER", "VOLCANO", "LAVA", "MAGMA", "EARTHQUAKE", "TSUNAMI", "AVALANCHE", "LANDSLIDE", "SPRING", "STREAM", "MOUTH", "DELTA", "COAST", "BEACH", "ISLAND", "PENINSULA", "CLIFF", "ROCK", "CANYON", "VALLEY", "PLATEAU", "PLAIN", "DESERT", "STEPPE", "SAVANNA", "RAINFOREST", "TROPICS", "POLE", "RANGE", "ALPS", "HIMALAYAS", "ANDES", "ROCKIES", "PYRENEES", "CARPATHIANS", "JUNGLE", "FOREST", "WOODLAND", "PARK", "GARDEN", "GREENHOUSE", "ARBOR", "PAVILION", "FOUNTAIN", "POND", "POOL", "BASIN", "SLIDE", "SWING", "CLIMBER", "SANDBOX", "PLAYGROUND", "TOY", "PUZZLE", "BLOCKS", "CLAY", "COLORINGBOOK", "PAINT", "BRUSH", "WATERCOLOR", "CRAYON", "MARKER", "HIGHLIGHTER", "ROLLERBALL", "QUILL", "INK", "NOTEPAD", "CLIPBOARD", "DRAWING", "SKETCH", "PAINTING", "FRAME", "CANVAS", "EASEL", "IRON", "VACUUM", "BROOM", "DUSTPAN", "MOP", "BUCKET", "SOAP", "TOWEL", "TOOTHBRUSH", "TOOTHPASTE", "SHOWER", "BATHTUB", "TOILET", "CABINET", "WARDROBE", "BED", "PILLOW", "BLANKET", "MATTRESS", "FLOORLAMP", "DESK", "BOOKSHELF", "SHELF", "SOFA", "ARMCHAIR", "HAMMOCK", "CURTAIN", "DRAPE", "BLIND", "WINDOWSILL", "FLOWERPOT", "GARDENFENCE", "SHED", "GREENHOUSE", "GATE", "GARAGE", "CARPORT", "CAR", "BIKE", "SCOOTER", "MOTORBIKE", "BUS", "TRAIN", "PLANE", "SHIP", "BOAT", "CANOE", "SUBMARINE", "TRACTOR", "EXCAVATOR", "CRANE", "TRUCK", "TAXI", "TRAFFICLIGHT", "ROAD", "BRIDGE", "TUNNEL", "CROSSROAD", "ROUNDABOUT", "CURB", "CROSSWALK", "SIGN", "TRAFFIC", "POLICE", "FIRETRUCK", "AMBULANCE", "HOSPITAL", "DOCTOR", "PHARMACY", "MEDICINE", "BANDAGE", "PLASTER", "THERMOMETER", "FEVER", "COUGH", "COLD", "VACCINE", "VIRUS", "BACTERIA", "MICROSCOPE", "LAB", "SCHOOL", "CLASSROOM", "TEACHER", "STUDENT", "BLACKBOARD", "CHALK", "TEXTBOOK", "NOTEBOOK", "PENCILCASE", "SCHOOLYARD", "GYM", "SPORT", "BALL", "GOAL", "NET", "SLED", "SKI", "SNOWBOARD", "SNOWBALL", "SNOWMAN", "ICERINK", "SKATE", "WALL", "FLOOR", "CEILING", "LIGHT", "ELECTRICITY", "SOCKET", "CABLE", "SWITCH", "BATTERY", "CHARGER", "ROBOT", "PRINTER", "SCANNER", "MONITOR", "KEYBOARD", "MOUSE", "SPEAKER", "HEADPHONES", "MICROPHONE", "ALARM", "CALENDAR", "JOURNAL", "FOLDER", "FILES", "GLASSES", "SUNGLASSES", "UMBRELLA", "JACKET", "COAT", "SHOES", "BOOTS", "PANTS", "SKIRT", "SHIRT", "TSHIRT", "SWEATER", "HAT", "GLOVES", "SCARF", "BELT", "BUTTON", "ZIPPER", "LADDER", "TOOLBOX", "HAMMER", "SCREWDRIVER", "WRENCH", "NAIL", "SCREW", "PLIER", "TAPE", "MEASURER", "LEVEL", "DRILL", "SAW", "CHISEL", "WORKBENCH", "GOGGLES", "GLOVES", "HELMET", "APRIL", "AUGUST", "OCTOBER"};
    for (int j = 0; words[j] != '\0'; j++) {
        fprintf(english, "%s\n", words[j]);
    }
    fclose(english);

    //Highscore
    highscore = fopen("highscore.txt", "w");
    if (!highscore)
        printf("An error occurred while creating this file: %s\n", "highscore.txt");
    fputc('0', highscore);
    fclose(highscore);
}

void checkFiles() {
    FILE *german = fopen("woerter.txt", "r");
    FILE *english = fopen("words.txt", "r");
    FILE *highscore = fopen("highscore.txt", "r");

    if (!german || !english || !highscore)
        makeFiles();
    fclose(german);
    fclose(english);
    fclose(highscore);
}

int getPreviousStreak() {
    int ziffer[21],faktor=1,len=0,i,zahl=0;
    char c;

    FILE *highscore;

    highscore = fopen("highscore.txt","r");
    if(highscore==NULL) printf("An error occurred while creating this file %s\n", highscore);
    else while((c=fgetc(highscore))!=EOF)
         {
            ziffer[len]=c-48;
            len++;
         }
     for(i=len-1;i>=0;i--)
     {
         zahl=zahl+ziffer[i]*faktor;
         faktor=faktor*10;
     }

    fclose(highscore);

    return zahl;
}

int saveStreak(int streak) {
    FILE *highscore;
    int best = getPreviousStreak();

    if (best >= streak) {
        return 0;
    }
    else {
        highscore = fopen("highscore.txt", "w");
        if (!highscore)
            printf("An error occurred while creating this file: %s\n", highscore);
        fprintf(highscore, "%d", streak);
        fclose(highscore);
        return 1;
    }
}

//Let the Player choose the preferred Language
Language chooseLanguage() {
    int choice;
    printf("Sprache wählen / Choose Language:\n");
    printf("1. Deutsch\n");
    printf("2. English\n");
    printf(">> ");
    scanf("%d", &choice);
    system("cls");
    return (choice == 1) ? DE : EN;
}

//Print a little Welcome Message
void printWelcome(Language lang) {
    if (lang == DE)
        printf("\nWillkommen zu Hangman – der Deutschen Variante!\n");
    else
        printf("\nWelcome to Hangman – the English variant!\n");
}

//Print actual Streak
void printStreak(int streak) {
    set_color(12);
    printf("\t\t\t\tStreak: %d", streak);
    set_color(14);
    printf("\tHighscore: %d", getPreviousStreak());
    set_color(7);
}

//print the actual word to be guessed
void printWordState(char *word, char *guessed, int streak) {
    for (int i = 0; word[i]; i++) {
        if (guessed[i])
            printf("%c ", word[i]); //when letter is guessed
        else
            printf("_ "); //when letter is NOT guessed
    }
    printStreak(streak);
    printf("\n");
}

//Check if the word is guessed
int isWordGuessed(char *word, char *guessed) {
    for (int i = 0; word[i]; i++) {
        if (!guessed[i])
            return 0;
    }
    return 1;
}

//change the letters to be lower case
void toLowerStr(char *str) {
    for (int i = 0; str[i]; i++)
        str[i] = tolower(str[i]);
}

//check if the letter was already guessed
int alreadyGuessed(char ch, char *wrongLetters, int wrongCount) {
    for (int i = 0; i < wrongCount; i++)
        if (wrongLetters[i] == ch)
            return 1;
    return 0;
}


int main() {
    //Change Codepage
    system("chcp 1252");
    system("cls");

    //Initialize Random
    srand(time(NULL));

    checkFiles();


    Language lang;
    char playAgain;

    int streak = 0;

    lang = chooseLanguage(); //set language
    printWelcome(lang);

    //Game-Loop
    do {
        //set file to choose from
        const char *filename = (lang == DE) ? "woerter.txt" : "words.txt";
        char words[MAX_WORDS][MAX_LENGTH]; //array for the words
        int wordCount;


        //load the file and words
        loadWords(filename, words, &wordCount);

        //set random word from list
        char *word = words[rand() % wordCount];
        toLowerStr(word); //set letters to lower case
        int len = strlen(word); //get length of the word

        char guessed[MAX_LENGTH] = {0};  //Array for guessed letters and spaces
        int tries = 0;
        char wrongLetters[26] = {0}; //Array for wrong letters
        int wrongCount = 0;

        //Round-Loop
        while (tries < MAX_TRIES && !isWordGuessed(word, guessed)) {
            printf("\n");
            printWordState(word, guessed, streak);  // print underlines
            printHangman(tries); //print hangman

            //print wrong letters
            printf("%s", (lang == DE) ? "Falsche Buchstaben: " : "Wrong letters: ");
            for (int i = 0; i < wrongCount; i++)
                printf("%c ", wrongLetters[i]);
            printf("\n");

            //Players guess turn
            char guess;
            printf("%s", (lang == DE) ? "Rate einen Buchstaben: " : "Guess a letter: ");
            fflush(stdin);
            scanf(" %c", &guess);
            guess = tolower(guess); // Set Players letter to lower case

            //Compare the Player letter to the searched word
            int found = 0;
            for (int i = 0; i < len; i++) {
                if (word[i] == guess && !guessed[i]) {
                    guessed[i] = 1;
                    found = 1;
                }
                if (word[i] == guess && guessed[i]) {
                    found = 1;
                }
            }

            //if no comparison is found and letter wasn't guessed before add try
            if (!found && !alreadyGuessed(guess, wrongLetters, wrongCount)) {
                wrongLetters[wrongCount++] = guess;
                tries++;
            }
            system("cls");
        }

        printf("\n");
        printWordState(word, guessed, streak); //print the actual word to be guessed in his current state
        printHangman(tries); //print hangman

        //if the word is guessed correctly
        if (isWordGuessed(word, guessed)) {
            set_color(14);
            printf("%s %s\n", (lang == DE) ? "Glückwunsch! Du hast das Wort erraten:" : "Congratulations! You guessed the word:", word);
            streak++;
            set_color(7);
        }
        //if the word couldn't be guessed in time
        else {
            printf("%s %s\n\n", (lang == DE) ? "Game Over! Das Wort war:" : "Game Over! The word was:", word);

            printf("%s %d\n", (lang == DE) ? "Deine Streak ist:" : "Your Streak is:", streak);

            //Save new Streak if higher
            if (saveStreak(streak)) {
                set_color(2);
                printf("\n%s %d\n", (lang == DE) ?  "Du hast deinen Highscore geschlagen!!\nNeuer Highscore:" : "You beat your Highscore!!\nNew Highscore:", streak);
            }
            else {
                set_color(12);
                printf("\n%s %d\n", (lang == DE) ? "Leider kein neuer Highscore, dein Highscore beträgt:" : "No new Highscore! Your Highscore is: ", getPreviousStreak());
            }
            //Reset Streak
            streak = 0;
            set_color(7);
        }


        //Let the Player choose to play again
        printf("%s", (lang == DE) ? "\nNochmal spielen? (j/n): " : "\nPlay again? (y/n): ");
        fflush(stdin);
        scanf(" %c", &playAgain);
        system("cls");
    } while (playAgain == 'j' || playAgain == 'y');

    //If Player does'nt want to play anymore, farewell him
    system("cls");
    printf("\n\n\n\t\t\t\t%s\n", (lang == DE) ? "Danke fürs Spielen!" : "Thanks for playing!");
    Sleep(1500);
    return 0;
}
