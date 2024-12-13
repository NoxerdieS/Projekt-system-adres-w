# Projekt system adresow
Ogólny opis implementacji systemu zarządzającego zbiorem adresów osób, z automatycznym grupowaniem po zainteresowaniach:

## 1. Funkcjonalności systemu

**Dodawanie osób:**
Umożliwia użytkownikowi wprowadzenie danych osobowych (imię, adres, email) i listy zainteresowań.
Dane przechowywane w odpowiedniej strukturze danych, np. liście obiektów.

**Usuwanie osób:**
Pozwala użytkownikowi usunąć osobę z bazy na podstawie unikalnego identyfikatora, np. adresu email.

**Edycja danych osoby:**
Możliwość aktualizacji danych osobowych i zainteresowań.

**Wyświetlanie listy osób:**
Wyświetla listę wszystkich zapisanych osób w przejrzysty sposób, z możliwością filtrowania po określonym parametrze (np. zainteresowaniach).

**Automatyczne grupowanie po zainteresowaniach:**
Tworzenie grup osób na podstawie wspólnych zainteresowań.
Algorytm analizuje listy zainteresowań i grupuje osoby z co najmniej jednym wspólnym zainteresowaniem.

**Eksport i import danych:**
Zapis i odczyt bazy danych z pliku w formacie tekstowym lub CSV, co pozwala na trwałe przechowywanie informacji.

## 2. Kluczowe komponenty implementacji

**Struktury danych:**
Jako struktura zostanie użyty struct lub class.
Przechowywanie danych o osobach w formie listy lub tablicy.

**Logika grupowania:**
Wyszukiwanie osób z co najmniej jednym wspólnym zainteresowaniem.
Tworzenie dynamicznych grup na podstawie podobieństwa zainteresowań, np. przez porównanie zestawów zainteresowań dwóch osób.

**Obsługa plików:**
Zapisywanie danych w pliku txt, gdzie każda linia zawiera dane osoby (imię, nazwisko, adres, email, zainteresowania rozdzielone przecinkami).
Wczytywanie danych z pliku i ich walidacja.

**Interfejs użytkownika:**
Menu tekstowe umożliwiające wybór opcji (dodanie osoby, usunięcie, wyświetlenie grup, zapis/odczyt z pliku).
Obsługa wejścia i walidacja wprowadzonych danych.

**Moduł wyszukiwania:**
Filtrowanie osób na podstawie wybranych kryteriów, np. wyszukiwanie wszystkich osób interesujących się "programowaniem".

**Modularność:**
Podział kodu na logiczne moduły: osobna klasa dla osób (Person), obsługi bazy danych (Database), i logiki grupowania (GroupManager).
