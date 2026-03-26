#include <iostream> //nhap/ xuat 
#include <vector> // mo rong dung luong( do rong, dai )
#include <fstream>// lay du lieu tu file
#include <sstream>// chuyen doi chuoi-> so 
#include <iomanip>// dinh dang du lieu dau ra
#include <ctime>// xu ly du lieu thoi gian

using namespace std;

class Room{
public:
    string roomId;
    int capacity;
    double pricePerMonth;

    Room(){}
    Room(string id,int cap,double price){
        roomId=id;
        capacity=cap;
        pricePerMonth=price;
    }
};

class Student{
public:
    string studentId;
    string name;
    string phone;

    Student(){}
    Student(string id,string n,string p){
        studentId=id;
        name=n;
        phone=p;
    }
};

class Stay{
public:
    string stayId;
    string studentId;
    string roomId;
    string startDate;
    string endDate;
    string status;

    Stay(){}
    Stay(string id,string sid,string rid,string start,string end,string st){
        stayId=id;
        studentId=sid;
        roomId=rid;
        startDate=start;
        endDate=end;
        status=st;
    }
};

class DormService{

private:

    vector<Room> rooms; // tang giam do dai rong khong can dinh dang truoc
    vector<Student> students;
    vector<Stay> stays;

public: 

    /* ======================
        UTILITY FUNCTIONS
    ====================== */

    Room* findRoom(string id){
        for(auto &r:rooms)
            if(r.roomId==id) return &r;
        return nullptr;
    }

    Student* findStudent(string id){
        for(auto &s:students)
            if(s.studentId==id) return &s;
        return nullptr;
    }

    Stay* findActiveStay(string studentId){
        for(auto &s:stays)
            if(s.studentId==studentId && s.status=="Active")
                return &s;
        return nullptr;
    }

    int countActiveInRoom(string roomId){
        int count=0;
        for(auto &s:stays)
            if(s.roomId==roomId && s.status=="Active")
                count++;
        return count;
    }

    /* ======================
        LOAD / SAVE CSV    // HOANG LAN
    ====================== */

    void loadRooms(){
        ifstream f("rooms.csv");
        if(!f) return;

        string line;
        while(getline(f,line)){
            stringstream ss(line);

            string id,cap,price;

            getline(ss,id,',');
            getline(ss,cap,',');
            getline(ss,price,',');

            rooms.push_back(
                Room(id,stoi(cap),stod(price))
            );
        }
        f.close();
    }

    void loadStudents(){
        ifstream f("students.csv");
        if(!f) return;

        string line;
        while(getline(f,line)){
            stringstream ss(line);

            string id,name,phone;

            getline(ss,id,',');
            getline(ss,name,',');
            getline(ss,phone,',');

            students.push_back(
                Student(id,name,phone)
            );
        }
        f.close();
    }

    void loadStays(){
        ifstream f("stays.csv");
        if(!f) return;

        string line;
        while(getline(f,line)){
            stringstream ss(line);

            string a,b,c,d,e,fv;

            getline(ss,a,',');
            getline(ss,b,',');
            getline(ss,c,',');
            getline(ss,d,',');
            getline(ss,e,',');
            getline(ss,fv,',');

            stays.push_back(
                Stay(a,b,c,d,e,fv)
            );
        }
        f.close();
    }

    void saveRooms(){
        ofstream f("rooms.csv");
        for(auto &r:rooms)
            f<<r.roomId<<","<<r.capacity<<","<<r.pricePerMonth<<"\n";
    }

    void saveStudents(){
        ofstream f("students.csv");
        for(auto &s:students)
            f<<s.studentId<<","<<s.name<<","<<s.phone<<"\n";
    }

    void saveStays(){
        ofstream f("stays.csv");
        for(auto &s:stays)
            f<<s.stayId<<","<<s.studentId<<","<<s.roomId<<","
            <<s.startDate<<","<<s.endDate<<","<<s.status<<"\n";
    }

    void saveAll(){
        saveRooms();
        saveStudents();
        saveStays();
    }

    void loadAll(){
        loadRooms();
        loadStudents();
        loadStays();
    }

    /* ======================
        BASIC DATA ADD
    ====================== */

    void addRoom(){
        string id;
        int cap;
        double price;

        cout<<"Room ID: ";cin>>id;

        if(findRoom(id)){
            cout<<"Room exists\n";
            return;
        }

        cout<<"Capacity: ";cin>>cap;
        cout<<"Price per month: ";cin>>price;

        rooms.push_back(Room(id,cap,price));
        saveRooms();
    }

    void addStudent(){
        string id,name,phone;

        cout<<"Student ID: ";cin>>id;

        if(findStudent(id)){
            cout<<"Student exists\n";
            return;
        }

        cin.ignore();
        cout<<"Name: ";
        getline(cin,name);

        cout<<"Phone: ";
        getline(cin,phone);

        students.push_back(
            Student(id,name,phone)
        );

        saveStudents();
        cout<<"Student added\n";
    }

    /* ======================
        METHOD: assignRoom // BINH MINH  
    ====================== */

    bool assignRoom(string studentId,string roomId,string startDate){

        //1 Check student & room exist

        if(!findStudent(studentId) || !findRoom(roomId))
            return false;

        //2 Check student has no Active stay

        if(findActiveStay(studentId))
            return false;

        //3 Count Active < capacity

        Room* r=findRoom(roomId);

        if(countActiveInRoom(roomId)>=r->capacity)
            return false;

        //4 Create stay

        string stayId="ST"+to_string(stays.size()+1);

        stays.push_back(
            Stay(stayId,studentId,roomId,startDate,"","Active")
        );

        saveStays();
        return true;
    }

    /* ======================
        METHOD: moveRoom  // BINH MINH  
    ====================== */

    bool moveRoom(string studentId,string newRoomId,string date){

        //1 Find Active stay

        Stay* oldStay=findActiveStay(studentId);
        if(!oldStay) return false;

        //2 Check capacity

        Room* r=findRoom(newRoomId);
        if(!r) return false;

        if(countActiveInRoom(newRoomId)>=r->capacity)
            return false;

        //3 End old stay

        oldStay->endDate=date;
        oldStay->status="Ended";

        //4 Create new stay

        string stayId="ST"+to_string(stays.size()+1);

        stays.push_back(
            Stay(stayId,studentId,newRoomId,date,"","Active")
        );

        saveStays();
        return true;
    }

    /* ======================
        METHOD: checkout    // HOANG LAN
    ====================== */

    bool checkout(string studentId,string date){

        //1 Find Active stay

        Stay* st=findActiveStay(studentId);
        if(!st) return false;

        //2 End stay

        st->endDate=date;
        st->status="Ended";

        saveStays();
        return true;
    }

    /* ======================
        METHOD: roomStatus   // DUC MINH
    ====================== */

    void roomStatus(string roomId){

    // ===== CASE 1: ALL ROOM =====
    if(roomId == "ALL"){

        for(auto &r : rooms){

            cout << "\n=== Room " << r.roomId << " ===\n";

            int count = 0;

            for(auto &s : stays){

                if(s.roomId == r.roomId && s.status == "Active"){

                    Student* st = findStudent(s.studentId);

                    if(st){
                        cout << st->studentId << " "
                             << st->name << " "
                             << st->phone << "\n";
                    }

                    count++;
                }
            }

            if(count == 0){
                cout << "No students\n";
            }

            cout << "Capacity: " << count << "/" << r.capacity << "\n";
        }

        return;
    }

    // ===== CASE 2: SINGLE ROOM =====
    cout << "Students in room " << roomId << "\n";

    int count = 0;

    for(auto &s: stays){

        if(s.roomId == roomId && s.status == "Active"){

            Student* st = findStudent(s.studentId);

            if(st){
                cout << st->studentId << " "
                     << st->name << " "
                     << st->phone << "\n";
            }

            count++;
        }
    }

    if(count == 0){
        cout << "No students\n";
    }

    Room* r = findRoom(roomId);
    if(r){
        cout << "Capacity: " << count << "/" << r->capacity << "\n";
    }
}

    /* ======================
        METHOD: occupancyReport // DUC MINH
    ====================== */

    string occupancyReport(){

        stringstream report;

        report<<"Room | Used | Cap | Free | Percent\n";

        for(auto &r:rooms){

            int used=countActiveInRoom(r.roomId);
            int free=r.capacity-used;

            double percent=
            (double)used/r.capacity*100;

            report<<r.roomId<<" "
            <<used<<" "
            <<r.capacity<<" "
            <<free<<" "
            <<fixed<<setprecision(1)
            <<percent<<"%\n";
        }

        return report.str();
    }

    /* ======================
        EXTENSION 1
        calcMonthlyBill    // HOANG LAN
    ====================== */

    double calcMonthlyBill(string studentId, string currentDate){

    double amount = 0;

    // parse date 
    int cy, cm, cd;
    sscanf(currentDate.c_str(), "%d-%d-%d", &cy, &cm, &cd);

    for(auto &s : stays){

        if(s.studentId != studentId)
            continue;

        Room* r = findRoom(s.roomId);
        if(!r) continue;

        // parse start
        int sy, sm, sd;
        sscanf(s.startDate.c_str(), "%d-%d-%d", &sy, &sm, &sd);

        // parse end
        int ey, em, ed;

        if(s.endDate == "" || s.status == "Active"){
            ey = cy; em = cm; ed = cd; // chua checkout
        }else{
            sscanf(s.endDate.c_str(), "%d-%d-%d", &ey, &em, &ed);
        }

        //1 month = 30 days
        int totalDays =
            (ey - sy) * 360 +
            (em - sm) * 30 +
            (ed - sd);

        if(totalDays < 0) continue;

        double pricePerDay = r->pricePerMonth / 30;

        amount += pricePerDay * totalDays;
    }

    return amount;
}

    /* ======================
        EXTENSION 2
        overCapacityAlert    // BINH MINH
    ====================== */

    vector<string> overCapacityAlert(){

        vector<string> alert;

        for(auto &r:rooms){

            int count=countActiveInRoom(r.roomId);

            if(count>r.capacity)
                alert.push_back(r.roomId);
        }

        return alert;
    }

    /* ======================
        EXTENSION 3
        exportDormReport     // DUC MINH
    ====================== */   

    bool exportDormReport(string filename){

        ofstream f(filename);

        if(!f) return false;

        for(auto &r:rooms){

            int used=countActiveInRoom(r.roomId);

            f<<"Room "<<r.roomId
            <<" ("<<used<<"/"<<r.capacity<<")\n";

            for(auto &s:stays){

                if(s.roomId==r.roomId && s.status=="Active"){

                    Student* st=findStudent(s.studentId);

                    f<<st->studentId<<","
                    <<st->name<<","
                    <<st->phone<<"\n";
                }
            }

            f<<"\n";
        }

        f.close();
        return true;
    }

};

/* ======================
        MAIN MENU    // DUC MINH
====================== */

int main(){

    DormService dorm;
    dorm.loadAll();

    int choice;

    do{

        cout<<"\n===== DORM MANAGER =====\n";

        cout<<"1 Add Room\n";
        cout<<"2 Add Student\n";
        cout<<"3 Assign Room\n";
        cout<<"4 Move Room\n";
        cout<<"5 Checkout\n";
        cout<<"6 Room Status\n";
        cout<<"7 Occupancy Report\n";
        cout<<"8 Monthly Bill\n";
        cout<<"9 Export Report\n";
        cout<<"0 Exit\n";

        cin>>choice;
        if(choice==1){
		dorm.addRoom();
		} 
        if(choice==2){
		dorm.addStudent();
		}

        if(choice==3){

            string s,r,d;
            cout<<"StudentId:";cin>>s;
            cout<<"RoomId:";cin>>r;
            cout<<"StartDate:";cin>>d;

            if(dorm.assignRoom(s,r,d))
                cout<<"Assign OK\n";
            else
                cout<<"Assign FAIL\n";
        }

        if(choice==4){

            string s,r,d;
            cout<<"StudentId:";cin>>s;
            cout<<"NewRoom:";cin>>r;
            cout<<"Date:";cin>>d;

            if(dorm.moveRoom(s,r,d))
                cout<<"Move OK\n";
            else
                cout<<"Move FAIL\n";
        }

        if(choice==5){

            string s,d;
            cout<<"StudentId:";cin>>s;
            cout<<"Date:";cin>>d;

            if(dorm.checkout(s,d))
                cout<<"Checkout OK\n";
            else
                cout<<"Checkout FAIL\n";
        }

        if(choice==6){
		    string r;
		    cout<<"RoomId (or ALL): ";
		    cin>>r;
			
			dorm.roomStatus(r);
}

        if(choice==7){

            cout<<dorm.occupancyReport();
        }

       if(choice==8){
	   string s, date;

    cout<<"StudentId:"; cin>>s;
    cout<<"Current Date (YYYY-MM-DD): "; cin>>date;

    cout<<fixed<<setprecision(0);
    cout<<"Bill="
    <<dorm.calcMonthlyBill(s,date)
    <<endl;
}

        if(choice==9){

            string f;
            cout<<"File:";
            cin>>f;

            if(dorm.exportDormReport(f))
                cout<<"Export OK\n";
            else
                cout<<"Export FAIL\n";
        }

    }while(choice!=0);

    dorm.saveAll();
}
