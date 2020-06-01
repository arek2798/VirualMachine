#include <iostream>
#include <fstream>
#include <windows.h>

using namespace std;

#define mask_op 0b0000000000001111
#define mask_r1 0b0000001111110000
#define mask_r2 0b1111110000000000

struct op_struct {
    unsigned short result;
    int num;
};

void about() { // informacje o programie
    system("cls");
    cout << "O programie" << endl << endl;
    cout << "Program pozwala stworzyc i zapisac w pliku binarnym program na maszyne wirtualna oraz umozliwia uruchomienie zapisanego pliku programu. Program pozwala rozwiazywac podstawowe dzialania arytmetyczne(dodawanie, odejmowanie, mnozenie, dzielenie z reszta), kopiowanie, porownywanie, czytanie z klawiatury oraz wypisywanie na ekran." << endl << endl;
    system("pause");
}

void operationList() { //lista mozliwych operacji i warunki wykonania skoku
    cout << "Instrukcja             | Operacja | Opis" << endl;
    cout << "----------------------------------------------------------------------------------------------------" << endl;
    cout << "Dodaj                  |    0     | R1=R1+R2 " << endl;
    cout << "Odejmij                |    1     | R1=R1-R2 " << endl;
    cout << "Mnozenie               |    2     | R1=R1*R2 " << endl;
    cout << "Dziel z reszta         |    3     | R1=R1/R2 oraz R2=R1%R2" << endl;
    cout << "Porownaj               |    4     | Wynik R1-R2 nie jest zapisywany jedynie zmiasty w rejestrze flag" << endl;
    cout << "Kopiuj                 |    5     | R1=R2" << endl;
    cout << "Instrukcja skoku       |    6     | przeskok do innej instrukcji w zaleznosci od warunku(lista ponizej)" << endl;
    cout << "Wczytaj stala          |    7     | R1= 4 bajty zapisane po tej operacji" << endl;
    cout << "Czytaj z klawiatury    |    8     | do R1 wpisz liczbe z klawiatury " << endl;
    cout << "Wypisz na ekran        |    9     | z R1 wypisz liczbe na ekran " << endl;
    cout << "Koniec programu        |    10    | Zakoncz tworzenie programu " << endl;
    cout << "Zakoncz tworzenie pliku|    11    | Zakoñcz tworzenie pliku z programem " << endl;
    cout << "Pomoc                  |    12    | Wyswietlenie pomocy " << endl << endl;

    cout << "Warunki wykonania istrukcji skoku: " << endl << endl;
    cout << "0 - zawsze" << endl;
    cout << "1 - gdy R1 jest rowne zero" << endl;
    cout << "2 - gdy R1 jest rozne od zera" << endl;
    cout << "3 - gdy R1 jest dodatnie" << endl;
    cout << "4 - gdy R1 jest ujemne" << endl;
    cout << "5 - Gdy R1 jest nieujemne" << endl;
    cout << "6 - Gdy R1 jest niedodatnie" << endl << endl;
}

void help() { // wyswietlenie pomocy
    system("cls");
    cout << "Pomoc" << endl << endl;
    cout << "Tworzenie nowego programu:" << endl << endl;
    cout << "Program tworzymy na podstawie operacji oraz dodatkowych parametrow w postaci numerow rejetru w pamieci lub dodatkowych liczb skoku lub stalych." << endl << endl;
    cout << "Lista operacji: " << endl << endl;
    operationList();
    cout << "W czasie tworzenia programu dostepny jest dostep do pomocy -  liczba 12 wpisana w konsoli" << endl;
    system("pause");
}

void changeFlag(int value, int &flagZ, int &flagD, int &flagU) { // zmiana okreslonej flagi ze wzgledu na podana wartosc
    flagZ = 0, flagU = 0, flagD = 0;
    if(value == 0) flagZ = 1;
    else if(value > 0) flagD = 1;
    else if(value < 0) flagU = 1;
}

bool checkFlag(int value, int &flagZ, int &flagD, int &flagU) { //sprawadzanie czy spelniony jest warunek skoku
    bool canJump = false;

    switch (value) {
    case 0:
        canJump = true;
        break;
    case 1:
        if(flagZ == 1) canJump = true;
        break;
    case 2:
        if(flagZ == 0) canJump = true;
        break;
    case 3:
        if(flagD == 1) canJump = true;
        break;
    case 4:
        if(flagU == 1) canJump = true;
        break;
    case 5:
        if(flagD == 1 || flagZ == 1) canJump = true;
        break;
    case 6:
        if(flagU == 1 || flagZ == 1) canJump = true;
        break;
    }
    return canJump;
}

op_struct create_operation(unsigned short op) { // tworzenie operacji w postaci liczb w strukturze
    int R1, R2;
    int req;
    op_struct newOperation;
    switch(op) {
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
            cout << "Podaj numer pierwszego rejestru: ";
            cin >> R1;
            cout << "Podaj numer drugiego rejestru: ";
            cin >> R2;
            newOperation.result = (R2 << 10) + (R1 << 4) + op;
            break;
        case 6:
            cout << "Podaj warunek skoku: ";
            cin >> req;
            newOperation.result = (req << 4) + op;
            cout << "Podaj liczbe o jaka nalezy wykonac skok: ";
            cin >> newOperation.num;
            break;
        case 7:
            cout << "Podaj numer rejestru: ";
            cin >> R1;
            newOperation.result = (R1 << 4) + op;
            cout << "Podaj stala: ";
            cin >> newOperation.num;
            break;
        case 8:
        case 9:
            cout << "Podaj numer rejestru: ";
            cin >> R1;
            newOperation.result = (R1 << 4) + op;
            break;
        case 10:
            newOperation.result = op;
            cout << "Zakoncz program" << endl;
            break;
        default:
            cout << "Podana instrukcja nie istnieje, sprobuj ponownie" << endl;
            break;
    }
    return newOperation;
}

void create_program() { // tworzenie nowego programu na podstawie wykonanych operacji
    system("cls");
    bool endProgram = false;
    cout << "Podaj nazwe pliku (razem z rozszerzeniem): ";
    char fileName[100];
    cin >> fileName;
    ofstream file(fileName, ios::app | ios::binary);
    unsigned short op;
    op_struct newOp;
    while(!endProgram) {
        cout << "Podaj numer operacji: ";
        cin >> op;
        if(op >= 0 && op <= 9) {
            if(op == 6 || op == 7) {
                newOp = create_operation(op);
                char buf2bytes[2];
                buf2bytes[0] = static_cast<char>(newOp.result);         //zapis w formacie little endian
                buf2bytes[1] = static_cast<char>(newOp.result >> 8);
                file.write(buf2bytes, 2);
                char buf4bytes[4];
                buf4bytes[0] = static_cast<char>(newOp.num);
                buf4bytes[1] = static_cast<char>(newOp.num >> 8);       //zapis w formacie little endian
                buf4bytes[2] = static_cast<char>(newOp.num >> 16);
                buf4bytes[3] = static_cast<char>(newOp.num >> 24);
                file.write(buf4bytes, 4);
            } else {
                newOp = create_operation(op);
                char buf2bytes[2];
                buf2bytes[0] = static_cast<char>(newOp.result);
                buf2bytes[1] = static_cast<char>(newOp.result >> 8);
                file.write(buf2bytes, 2);
            }
        } else if(op == 10) {
            newOp = create_operation(op);
            char buf2bytes[2];
            buf2bytes[0] = static_cast<char>(newOp.result);
            buf2bytes[1] = static_cast<char>(newOp.result >> 8);
            file.write(buf2bytes, 2);
        } else if(op == 11) {
            endProgram = true;
        } else if(op == 12) {
            help();
            system("cls");
        }
    }
    file.close();
}

void processor(unsigned short operation[64], int memory[64], short &activeOp, int &flagZ, int &flagU, int &flagD, int &jumpAndConst) {
    unsigned short oneOp = operation[activeOp];
    int op = mask_op & oneOp;           //wyodrebnienie liczby odpowiadajacej operacji oraz numery rejestrow
    int R1 = (mask_r1 & oneOp) >> 4;    // koniunkcja 1&1=1
    int R2 = (mask_r2 & oneOp) >> 10;
    switch(op) {
        case 0:
            cout << "Dodawanie rejestrow nr. " << R1 << " oraz " << R2 << endl;
            memory[R1] = memory[R1] + memory[R2];
            changeFlag(memory[R1], flagZ, flagD, flagU);
            activeOp++;
            break;
        case 1:
            cout << "Odejmowanie rejestrow nr. " << R1 << " oraz " << R2 << endl;
            memory[R1] = memory[R1] - memory[R2];
            changeFlag(memory[R1], flagZ, flagD, flagU);
            activeOp++;
            break;
        case 2:
            cout << "Mnozenie rejestrow nr. " << R1 << " oraz " << R2 << endl;
            memory[R1] = memory[R1] * memory[R2];
            changeFlag(memory[R1], flagZ, flagD, flagU);
            activeOp++;
            break;
        case 3:
            cout << "Dzielenie rejestrow nr. " << R1 << " oraz " << R2 << endl;
            memory[R1] = memory[R1] / memory[R2];
            memory[R2] = memory[R1] % memory[R2];
            changeFlag(memory[R1], flagZ, flagD, flagU);
            activeOp++;
            break;
        case 4:
            cout << "Porownywanie rejestrow nr. " << R1 << " oraz " << R2 << endl;
            changeFlag(memory[R1] - memory[R2], flagZ, flagD, flagU);
            activeOp++;
            break;
        case 5:
            cout << "Skopiowanie rejsetru nr. " << R2 << " do rejestru numer " << R1 << endl;
            memory[R1] = memory[R2];
            activeOp++;
            break;
        case 6:
            if(checkFlag(R1, flagZ, flagD, flagU)) {
                int jump;
                jump = static_cast<int32_t>((operation[activeOp+1]) | (operation[activeOp+2] << 16));
                if(jump >= 0) {
                    activeOp+=(jump+2);
                    jumpAndConst++;
                } else {
                    activeOp = activeOp + (jump - jumpAndConst*2);
                    jumpAndConst = 0;
                }
                cout << "operacja skoku o " << jump << endl;
            } else activeOp+=3;
            break;
        case 7:
            cout << "Przypisanie stalej do " << R1 << endl;
            int _const;
            _const = static_cast<int32_t>((operation[activeOp+1]) | (operation[activeOp+2] << 16));
            memory[R1] = _const;
            activeOp += 3;
            jumpAndConst++;
            break;
        case 8:
            cout << "Podaj liczbe. Podana liczba zostanie wczytana do rejestru nr. " << R1 << endl;
            cin >> memory[R1];
            activeOp++;
            break;
        case 9:
            cout << "Wartosc rejestru o nr. " << R1 << " wynosi:" << endl;
            cout << memory[R1] << endl;
            activeOp++;
            break;
        case 10:
            cout << "Zakonczenie programu." << endl;
            activeOp = -1;
            break;
    }
}

void open_file(char fileName[100], bool &error) { // funkcja odczytujaca z bliku liczby 2bajtowe i zapisujaca je do tabeli
    system("cls");
    int memory[64];
    unsigned short int operation[64];
    int flagZ = 0, flagU = 0, flagD = 0;
    int jumpAndConst = 0;

    for(int i=0; i<64; i++) {
        memory[i] = 0;
        operation[i] = 0;
    }

    ifstream file(fileName, ios::in | ios::binary);

    char buf[2];
    int counter = 0;

    while(!file.eof()) {
        file.read(buf, 2);
        operation[counter++] = static_cast<int16_t>((buf[0]) | (buf[1] << 8));
    }

    short activeOp = 0;

    while(activeOp >= 0) {
        processor(operation, memory, activeOp, flagZ, flagU, flagD, jumpAndConst);
    }

    system("pause");

    file.close();
    error = false;
}

void open_program() { //funkcja sprawdzajaca czy plik istnieje i otwiera go
    system("cls");
    char fileName[100];
    bool error = true;

    while(error) {
        cout << "Podaj nazwe pliku z programem(lacznie z rozszerzeniem): ";
        cin >> fileName;
        ifstream file(fileName, ios::in | ios::binary);

        if(file.fail()) {
            cout << "Plik nie istnieje" << endl;
        } else {
            open_file(fileName, error);
            file.close();
        }
    }
}

void exampleOne() { // przyklad programu

    ofstream file("przyklad1.bin", ios::app | ios::binary);
    unsigned short op[11] = {8, 2052, 38, 2, 0, 10, 16, 25, 6, 65530, 65535};
    char buf2bytes[2];
    for(int i=0; i<11; i++) {
        buf2bytes[0] = static_cast<char>(op[i]);
        buf2bytes[1] = static_cast<char>(op[i] >> 8);
        file.write(buf2bytes, 2);
    }

    file.close();

    bool error = true;
    open_file((char *) & "przyklad1.bin", error);
}

void exampleTwo() { // przyklad programu

    ofstream file("przyklad2.bin", ios::app | ios::binary);
    unsigned short op[10] = {8, 24, 1028, 102, 3, 0, 9, 10, 25, 10};
    char buf2bytes[2];
    for(int i=0; i<10; i++) {
        buf2bytes[0] = static_cast<char>(op[i]);
        buf2bytes[1] = static_cast<char>(op[i] >> 8);
        file.write(buf2bytes, 2);
    }

    file.close();

    bool error = true;
    open_file((char *) & "przyklad2.bin", error);
}

char menu(bool &err) {
    int x;
    cout << "MASZYNA WIRTUALNA" << endl << endl;
    cout << "Wybierz opcje:" << endl;
    cout << "1. Stworz swoj program" << endl;
    cout << "2. Wczytaj istniejacy program" << endl;
    cout << "3. O programie" << endl;
    cout << "4. Pomoc" << endl;
    cout << "5. Przykladowy program dodajacy liczby, konczy dzialanie, gdy zostanie podane 0" << endl;
    cout << "6. Przykladowy program wypisujacy najwieksza z podanych dwoch liczb" << endl;
    cout << "7. Wyjdz" << endl << endl;
    if(err) {
        cout << "Podana opcja nie istnieje" << endl;
        err = false;
    }
    cout << "-> ";
    cin >> x;
    return x;
}

int main()
{
    int op = 0;
    int R1 = 0, R2 = 0;
    bool endApp = false;
    bool err = false;

    while(!endApp) {
        int x;
        x = menu(err);
        if(x == 1) {
            create_program();
        } else if(x == 2) {
            open_program();
        } else if(x == 3) {
            about();
        } else if(x == 4) {
            help();
        } else if(x == 5) {
            exampleOne();
        } else if(x == 6) {
            exampleTwo();
        } else if(x == 7) {
            endApp = true;
        } else {
            err = true;
        }
        system("cls");
    }

    return 0;
}
