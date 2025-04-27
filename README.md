# Climate Controller | Contrôleur de Climat

![Climate Controller Interface](CC.jpg)

[English](#english) | [Français](#français)

---

# English

## Overview
An Arduino-based climate control system for managing temperature, humidity, and lighting in enclosed environments.

### Key Features
- 🌡️ Temperature & humidity control
- 💡 Programmable lighting
- 🔄 Bilingual interface
- ⚡ 5-relay control system
- 💾 Persistent settings storage
- 🖥️ LCD interface

### Hardware Required
- Arduino (Uno/Mega/Nano)
- DHT22 sensor
- DS3231 RTC module
- 20x4 I2C LCD display (I2C address: 0x27)
- 5 momentary switches (for UP, DOWN, LEFT, RIGHT, SET buttons)
- 5 control relays
- Jumper wires
- Breadboard or PCB

### Quick Start
1. Connect components
2. Upload code
3. Configure settings via LCD
4. Monitor & enjoy automated control

## How-To Guide

### Installation

1. **Install Required Libraries**
   - Open Arduino IDE
   - Go to Tools > Manage Libraries...
   - Install the following libraries:
     - `LiquidCrystal_I2C` (by Frank de Brabander)
     - `DHT sensor library` (by Adafruit)
     - `RTClib` (by Adafruit)
     - `uEEPROMLib` (by Naguissa)
     - `Bounce2` (by Thomas O Fredericks)

2. **Download and Upload the Code**
   - Clone or download this repository
   - **IMPORTANT:** First upload the `ClimateControllerDateTimeEeprom` preset sketch
   - After that completes, open `ClimateController.ino` in Arduino IDE
   - Select your Arduino board from Tools > Board
   - Connect your Arduino board via USB
   - Click Upload

### Hardware Connections

**LCD Display (I2C)**
- VCC → Arduino 5V
- GND → Arduino GND
- SDA → Arduino A4 (Uno/Nano) or 20 (Mega)
- SCL → Arduino A5 (Uno/Nano) or 21 (Mega)

**DHT22 Sensor**
- VCC → Arduino 5V
- DATA → Arduino Digital Pin 7
- GND → Arduino GND

**DS3231 RTC Module**
- VCC → Arduino 5V
- GND → Arduino GND
- SDA → Arduino A4 (Uno/Nano) or 20 (Mega)
- SCL → Arduino A5 (Uno/Nano) or 21 (Mega)

**Control Buttons**
- UP button → Arduino Digital Pin 9 (other end to GND)
- DOWN button → Arduino Digital Pin 10 (other end to GND)
- LEFT button → Arduino Digital Pin 11 (other end to GND)
- RIGHT button → Arduino Digital Pin 12 (other end to GND)
- SET button → Arduino Digital Pin 8 (other end to GND)

**Relay Modules**
- VCC → Arduino 5V
- GND → Arduino GND
- IN1 (Heating) → Arduino Digital Pin 2
- IN2 (Cooling) → Arduino Digital Pin 3
- IN3 (Humidifier) → Arduino Digital Pin 4
- IN4 (Dehumidifier) → Arduino Digital Pin 5
- IN5 (Light) → Arduino Digital Pin 6

### Using the Climate Controller

1. **Navigating Menus**
   - Use UP/DOWN buttons to navigate through menu options
   - Use LEFT/RIGHT buttons to switch between menu screens
   - Press SET button to select an option or save settings

2. **Setting Date and Time**
   - Navigate to the Time/Date menu
   - Use UP/DOWN buttons to change values
   - Press SET or RIGHT button to save and move to the next field
   - Fields include: hour, minute, day, month, year

3. **Setting Temperature Controls**
   - Navigate to the Temperature menu
   - Set desired temperature and hysteresis (temperature range)
   - Controller will automatically manage heating and cooling relays

4. **Setting Humidity Controls**
   - Navigate to the Humidity menu
   - Set desired humidity level and hysteresis
   - Controller will manage humidifier and dehumidifier relays

5. **Setting Light Schedule**
   - Navigate to the Light menu
   - Set ON and OFF times for the lighting system
   - Light relay will activate according to the schedule

6. **Changing Language**
   - Navigate to the Language menu
   - Select English or French

### Troubleshooting

- **LCD Not Displaying**: Check I2C address (default: 0x27) and connections
- **Incorrect Readings**: Verify DHT22 sensor connections and placement
- **Time/Date Issues**: Check RTC module connections and battery
- **Relays Not Switching**: Verify relay connections and test manually
- **Button Navigation Problems**: Check button wiring and debounce settings

---

# Français

## Aperçu
Système de contrôle climatique basé sur Arduino pour gérer la température, l'humidité et l'éclairage dans des environnements fermés.

### Caractéristiques Principales
- 🌡️ Contrôle température & humidité
- 💡 Éclairage programmable
- 🔄 Interface bilingue
- ⚡ Système à 5 relais
- 💾 Sauvegarde des paramètres
- 🖥️ Interface LCD

### Matériel Requis
- Arduino (Uno/Mega/Nano)
- Capteur DHT22
- Module RTC DS3231
- Écran LCD 20x4 I2C (adresse I2C: 0x27)
- 5 interrupteurs momentanés (pour les boutons HAUT, BAS, GAUCHE, DROITE, SET)
- 5 relais de contrôle
- Fils de raccordement
- Plaque d'essai ou PCB

### Démarrage Rapide
1. Connecter les composants
2. Téléverser le code
3. Configurer via LCD
4. Surveiller & profiter du contrôle automatisé

## Guide d'Utilisation

### Installation

1. **Installer les Bibliothèques Requises**
   - Ouvrir l'IDE Arduino
   - Aller à Outils > Gérer les Bibliothèques...
   - Installer les bibliothèques suivantes:
     - `LiquidCrystal_I2C` (par Frank de Brabander)
     - `DHT sensor library` (par Adafruit)
     - `RTClib` (par Adafruit)
     - `uEEPROMLib` (par Naguissa)
     - `Bounce2` (par Thomas O Fredericks)

2. **Télécharger et Téléverser le Code**
   - Cloner ou télécharger ce dépôt
   - **IMPORTANT:** D'abord téléverser le croquis préréglé `ClimateControllerDateTimeEeprom`
   - Après cela, ouvrir `ClimateController.ino` dans l'IDE Arduino
   - Sélectionner votre carte Arduino dans Outils > Type de Carte
   - Connecter votre carte Arduino via USB
   - Cliquer sur Téléverser

### Connexions Matérielles

**Écran LCD (I2C)**
- VCC → Arduino 5V
- GND → Arduino GND
- SDA → Arduino A4 (Uno/Nano) ou 20 (Mega)
- SCL → Arduino A5 (Uno/Nano) ou 21 (Mega)

**Capteur DHT22**
- VCC → Arduino 5V
- DATA → Arduino Pin Digital 2
- GND → Arduino GND

**Module RTC DS3231**
- VCC → Arduino 5V
- GND → Arduino GND
- SDA → Arduino A4 (Uno/Nano) ou 20 (Mega)
- SCL → Arduino A5 (Uno/Nano) ou 21 (Mega)

**Boutons de Contrôle**
- Bouton HAUT → Arduino Pin Digital 9 (autre extrémité à GND)
- Bouton BAS → Arduino Pin Digital 10 (autre extrémité à GND)
- Bouton GAUCHE → Arduino Pin Digital 11 (autre extrémité à GND)
- Bouton DROITE → Arduino Pin Digital 12 (autre extrémité à GND)
- Bouton SET → Arduino Pin Digital 8 (autre extrémité à GND)

**Modules Relais**
- VCC → Arduino 5V
- GND → Arduino GND
- IN1 (Chauffage) → Arduino Pin Digital 2
- IN2 (Refroidissement) → Arduino Pin Digital 3
- IN3 (Humidificateur) → Arduino Pin Digital 4
- IN4 (Déshumidificateur) → Arduino Pin Digital 5
- IN5 (Éclairage) → Arduino Pin Digital 6

### Utilisation du Contrôleur de Climat

1. **Navigation dans les Menus**
   - Utiliser les boutons HAUT/BAS pour naviguer dans les options du menu
   - Utiliser les boutons GAUCHE/DROITE pour passer d'un écran à l'autre
   - Appuyer sur le bouton SET pour sélectionner une option ou sauvegarder les paramètres

2. **Réglage de la Date et de l'Heure**
   - Naviguer jusqu'au menu Heure/Date
   - Utiliser les boutons HAUT/BAS pour changer les valeurs
   - Appuyer sur le bouton SET ou DROITE pour sauvegarder et passer au champ suivant
   - Les champs incluent: heure, minute, jour, mois, année

3. **Réglage des Contrôles de Température**
   - Naviguer jusqu'au menu Température
   - Définir la température souhaitée et l'hystérésis (plage de température)
   - Le contrôleur gérera automatiquement les relais de chauffage et de refroidissement

4. **Réglage des Contrôles d'Humidité**
   - Naviguer jusqu'au menu Humidité
   - Définir le niveau d'humidité souhaité et l'hystérésis
   - Le contrôleur gérera les relais d'humidificateur et de déshumidificateur

5. **Réglage du Calendrier d'Éclairage**
   - Naviguer jusqu'au menu Éclairage
   - Définir les heures ON et OFF pour le système d'éclairage
   - Le relais d'éclairage s'activera selon le calendrier défini

6. **Changement de Langue**
   - Naviguer jusqu'au menu Langue
   - Sélectionner Anglais ou Français

### Dépannage

- **LCD n'affiche rien**: Vérifier l'adresse I2C (par défaut: 0x27) et les connexions
- **Lectures incorrectes**: Vérifier les connexions et l'emplacement du capteur DHT22
- **Problèmes d'heure/date**: Vérifier les connexions du module RTC et sa pile
- **Relais ne commutent pas**: Vérifier les connexions des relais et tester manuellement
- **Problèmes de navigation par boutons**: Vérifier le câblage des boutons et les paramètres d'anti-rebond

---

## Support | Soutien | 
- 🌐 www.guillaumeguiss.com
- 📧 controller@guillaumeguiss.com

## License | Licence 
GNU General Public License
