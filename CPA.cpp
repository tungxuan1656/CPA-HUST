/*
Tính CPA
*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

typedef enum DiemHP {
	A=40, APLUS=40,  
	B=30, BPLUS=35,  
	C=20, CPLUS=25,  
	D=10, DPLUS=15, 
	F=0
} DiemHP;

typedef struct HocPhan {
	int sotin;
	char *mahp;
	DiemHP diemhp;
	struct HocPhan *next;
} HP;
// danh sach hoc phan cua tung hoc ki
// danh sach tat ca hoc phan
// => co 2 loai danh sach hoc phan

typedef struct HocKi {
	int hocki;
	HP *hocphan;
	float gpa;
	float cpa;
	struct HocKi *next;
} HK;

// dua con tro den ki tu thuoc bang chu cai tiep theo
void JumpToWord(FILE *input) { 
	int kytuhientai;
	while (kytuhientai!=EOF && !( // neu nhu ma khong nam trong bang chu cai thi doc tiep
			(kytuhientai>='0' && kytuhientai<='9') || 
			(kytuhientai>='A' && kytuhientai<='Z') || 
			(kytuhientai>='a' && kytuhientai<='z'))) {kytuhientai=fgetc(input);}
	// nam trong bang chu cai thì dau nhay lui lai 1 vi tri
	fseek(input, -1, SEEK_CUR);
	return;
}

// con tro hoc phan last la con tro tro den hoc phan cuoi cung trong danh sach hoc phan dang doc
void ThemVaoDSHocPhan(HP *&p_HocPhan, HP *&p_LastHP, int soTin, char *maHocPhan, DiemHP diemHocPhan) {
	HP *newnode = (HP *)malloc(sizeof(HP));
	newnode->sotin=soTin;
	newnode->mahp = maHocPhan;
	newnode->diemhp=diemHocPhan;
	newnode->next=NULL;
	if (p_HocPhan==NULL) { // neu chua co hoc phan nao
		p_HocPhan = newnode;
		p_LastHP = p_HocPhan;
	}
	else { // neu co roi thi them vao cuoi
		p_LastHP->next=newnode;
		p_LastHP = p_LastHP->next;
	}
}

// con tro last hoc ki la tro toi hoc ki cuoi cung dang duoc doc
void ThemVaoDSHocKi(HK* &p_HocKi, int hocKi, HK* &p_LastHK) {
	HK *newnode = (HK *)malloc(sizeof(HK));
	newnode->hocki= hocKi;
	newnode->hocphan = NULL;
	newnode->next= NULL;
	if (p_HocKi==NULL) {
		p_HocKi = newnode;
		p_LastHK = p_HocKi;
	}
	else {
		p_LastHK->next = newnode;
		p_LastHK=p_LastHK->next;
	}
}

// tinh GPA hoc ki duoc nhap vao
void TinhGPA(HK* &p_HocKi, int hocKi) { //tinh hoc ki nao dau vao hoc ki do
	// kiem tra xem co ton tai hoc ki day khong
	HK *p_hocki_temp = p_HocKi;
	if (p_hocki_temp == NULL) {
		printf("Khong ton tai du lieu hoc ki!");
		return;
	}
	while (p_hocki_temp->hocki!=hocKi && p_hocki_temp!=NULL) p_hocki_temp=p_hocki_temp->next;
	if (p_hocki_temp==NULL) {
		printf("Hoc ki khong ton tai!\n");
		return;
	}
	//neu co thi con tro p_hocki_temp se tro den hoc ki do

	HP *p_hocphan=p_hocki_temp->hocphan;
	int countsotin=0, S=0;

	while (p_hocphan!=NULL) {
		countsotin+=p_hocphan->sotin;
		S+=(p_hocphan->sotin)*(p_hocphan->diemhp);
		p_hocphan = p_hocphan->next;
	}
	p_hocki_temp->gpa=S*0.1/countsotin; // vi enum la so nguyen cho nen khi khoi tao da x10 nen bay gio chia 10
}

// kiem tra xem mon nay da tung hoc trong cac ki truoc chua
// neu da tung hoc thi xem bay gio diem co cao hon khong
// neu cao hon thi tra lai so diem da hoc truoc do
// neu thap hon thi tra lai -1
// chua tung hoc thi tra lai 0
int KiemTraHocLai(HK *p_HocKi, HK *p1, HP *p2) {
	HK *p_hocki_temp = p_HocKi;
	HP *p_hocphan_temp;

	// chi kiem tra cac ki dang truoc do'
	while (p_hocki_temp->hocki < p1->hocki) {
		p_hocphan_temp = p_hocki_temp->hocphan;
		while (p_hocphan_temp!=NULL) {
			// neu trung ma hoc phan
			if (strcmp(p_hocphan_temp->mahp, p2->mahp)==0) {
				if (p_hocphan_temp->diemhp < p2->diemhp) { // hoi xua hoc ngu hon
					return p_hocphan_temp->diemhp;
				}
				else return -1;
			}
			p_hocphan_temp=p_hocphan_temp->next;
		}
		p_hocki_temp=p_hocki_temp->next;
	}
	return -100;
}

void TinhCPA(HK* &p_HocKi, int hocKi) {
	// kiem tra xem hoc ki nhap vao co ton tai khong
	HK *p_hocki_temp = p_HocKi;
	if (p_hocki_temp == NULL) {
		printf("Khong ton tai du lieu hoc ki!");
		return;
	}
	/*
											    __________  ____  ____  ____ 
											   / ____/ __ \/ __ \/ __ \/ __ \
											  / __/ / /_/ / /_/ / / / / /_/ /
											 / /___/ _, _/ _, _/ /_/ / _, _/ 
											/_____/_/ |_/_/ |_|\____/_/ |_|  
	*/

	// khong duoc dao thu tu so sanh thanh (p_hocki_temp->hocki!=hocKi && p_hocki_temp!=NULL)
	// boi vi khi ma p_hocki_temp == NULL thi sau do no se tro den hocki la ko ton tai => ERROR
	while (p_hocki_temp!=NULL && p_hocki_temp->hocki!=hocKi) p_hocki_temp=p_hocki_temp->next;
	if (p_hocki_temp==NULL) {
		printf("Hoc ki khong ton tai!\n");
		return;
	}
	// p_hocki_temp dang tro den ki can tinh cpa
	int countsotin=0, S=0, temp=0;

	// p1, p2 la con tro xac dinh hoc phan dang xet
	HK *p1 = p_HocKi;
	HP *p2;

	// xet tat cac cac hoc phan tu dau cho den ki can tinh cpa
	
	while (p1!=NULL && p1->hocki<=hocKi) {
		p2=p1->hocphan;
		while (p2!=NULL) {
			temp = KiemTraHocLai(p_HocKi, p1, p2);
			// diem hoc lai cao hon diem hoi xua
			if (temp >= 0) {
				S=S-(p2->sotin)*temp + (p2->sotin)*(p2->diemhp);
			}
			else if (temp==-1); // hoc lai con ngu hon
			else if (temp==-100) { // mon nay chua hoc lai
					countsotin+=p2->sotin;
					S+=(p2->sotin)*(p2->diemhp);				
			}
			p2=p2->next;
		}
		p1=p1->next;
	}
	p_hocki_temp->cpa = S*0.1/countsotin;
}

int sotindaqua=0, sotindahoc=0, sotinno=0, sohocphandahoc=0,
	sohocphanA=0,
	sohocphanB=0, sohocphanBp=0, 
	sohocphanC=0, sohocphanCp=0, 
	sohocphanD=0, sohocphanDp=0, 
	sohocphanF=0;

void ThongKeTongHop(HK *p_HocKi) {
	if (p_HocKi == NULL) {
		printf("Khong ton tai du lieu hoc ki!");
		return;
	}

	HK *p1 = p_HocKi;
	HP *p2;
	int temp=0;

	// xet tat cac cac hoc phan tu dau cho den ki can tinh cpa
	while (p1!=NULL) {
		p2=p1->hocphan;
		while (p2!=NULL) {
			temp = KiemTraHocLai(p_HocKi, p1, p2);

			if (temp >= 0) { // diem hoc lai cao hon diem hoi xua
				switch(temp) {
					case 30: sohocphanB--;
						break;
					case 20: sohocphanC--;
						break;
					case 10: sohocphanD--;
						break;
					case 0: sohocphanF--;
							sotinno-=p2->sotin;
							goto LABEL;
						break;
					case 35: sohocphanBp--;
						break;
					case 25: sohocphanCp--;
						break;
					case 15: sohocphanDp--;
						break;
				}
			}
			else if (temp==-1) { // hoc lai con ngu hon

			} 
			else if (temp==-100){ // mon nay chua hoc lai
				sotindahoc+=p2->sotin;
				sohocphandahoc++;
				LABEL:
				switch(p2->diemhp) {
					case A: sohocphanA++;
						break;
					case B: sohocphanB++;
						break;
					case C: sohocphanC++;
						break;
					case D: sohocphanD++;
						break;
					case F: sohocphanF++;
						break;
					case BPLUS: sohocphanBp++;
						break;
					case CPLUS: sohocphanCp++;
						break;
					case DPLUS: sohocphanDp++;
						break;
				}
				if (p2->diemhp == F) sotinno+=p2->sotin;
				else sotindaqua+=p2->sotin;
			}
			p2=p2->next;
		}
		p1=p1->next;
	}
	//printf("%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t", sotindahoc, sotindaqua, sotinno, sohocphanA, sohocphanB, sohocphanBp, sohocphanC, sohocphanCp, sohocphanD, sohocphanDp, sohocphanF);
}

// ham nay doc file input -> luu du lieu vao 1 danh sach 2 chieu
HK *DocFileInput() {
	FILE *input = fopen("input.txt", "r");
	if (input == NULL) {
		printf("Mo tep tin that bai! Thoat chuong trinh");
		system("pause");
		exit(1);
	}

	// con tro last la tro toi doi tuong duoc xet den sau cung
	HK *p_hocki = NULL, *p_hocki_last=NULL;
	HP *p_hocphan_last;

	int sotin, kytuhientai;
	DiemHP diemhocphan;
	// dua dau nhay ve dau file
	rewind(input);


	// file .txt muon viet co dau phai luu o kieu utf-8
	// file utf-8 no ma hoa doan dau tien file la 1 dong deoo gi day ma no an di khong nhin thay duoc
	// phai doc file qua C/C++ nay moi thay duoc co 1 loat ki tu bat thuong duoc an di o dau file ma hoa dai dien cho utf-8
	// bo qua cac ki tu bat thuong ma dua dau nhay den dung ki tu dau tien minh nhin thay
	// di den cac ki tu minh co the nhin thay, la cac ki tu trong bang chu cai
	JumpToWord(input);
	/*
											    __________  ____  ____  ____ 
											   / ____/ __ \/ __ \/ __ \/ __ \
											  / __/ / /_/ / /_/ / / / / /_/ /
											 / /___/ _, _/ _, _/ /_/ / _, _/ 
											/_____/_/ |_/_/ |_|\____/_/ |_|  
	*/

	// vong lap de doc file
	while (1) {
		// doc doi tuong tiep theo la so integer
		fscanf(input, "%d", &kytuhientai);
		if (kytuhientai>20000) { // neu no la hoc ki
			if (kytuhientai%10==3) kytuhientai+=8; // gop cac hoc phan ki he vao ki sau
			// neu chua co hoc ki nao, hoac la hoc ki moi scanf là hoc ki moi thi them vao ds hoc ki
			if (p_hocki_last==NULL || kytuhientai != p_hocki_last->hocki)
				ThemVaoDSHocKi(p_hocki, kytuhientai, p_hocki_last);
			fgetc(input); // xuong dong
			continue; // tiep tuc doc dong duoi
		}
		// neu no khong phai la hoc ki thi no la so tin cua hoc phan
		sotin = kytuhientai;
		// dua dau nhay den cho diem hoc phan
		JumpToWord(input);
		int temp = fgetc(input); // luu lai la ABCDF
		kytuhientai=fgetc(input); // kiem tra xem no co phai la + khong
		if (kytuhientai!='+') { // khong phai A+ B+...
			switch(temp) {
				case 'A': diemhocphan = A;
					break;
				case 'B': diemhocphan = B;
					break;
				case 'C': diemhocphan = C;
					break;
				case 'D': diemhocphan = D;
					break;
				case 'F': diemhocphan = F;
					break;
			}
		}
		else { // A+ B+ thi khong co F
			switch(temp) {
				case 'A': diemhocphan = APLUS;
					break;
				case 'B': diemhocphan = BPLUS;
					break;
				case 'C': diemhocphan = CPLUS;
					break;
				case 'D': diemhocphan = DPLUS;
					break;
			}
		}
		// tiep tuc dua dau nhay den cho ma hoc phan
		JumpToWord(input);
		char *mahp = (char *)malloc(8*sizeof(char)); // chua cho cho \0
		// ma hoc phan co the co 6 hoac 7 ki tu
		for (int i=0; i<6; i++) {
			mahp[i] =fgetc(input);
		}
		// neu la 7 ki tu
		kytuhientai=fgetc(input);
		if (kytuhientai>='0' && kytuhientai<='9') {
			mahp[6]=kytuhientai;
			mahp[7]='\0';
		}
		else mahp[6]='\0';

		// dua cac thong tin da doc duoc tren dong nay gop lai thanh 1 hoc phan moi
		ThemVaoDSHocPhan(p_hocki_last->hocphan, p_hocphan_last, sotin, mahp, diemhocphan);

		kytuhientai = fgetc(input);
		// while (kytuhientai!=EOF && !( // neu nhu ma khong nam trong bang chu cai thi doc tiep
		// 	(kytuhientai>='0' && kytuhientai<='9') || 
		// 	(kytuhientai>='A' && kytuhientai<='Z') || 
		// 	(kytuhientai>='a' && kytuhientai<='z'))) {kytuhientai=fgetc(input);}

		while (kytuhientai!='\n' && kytuhientai!=EOF) {kytuhientai = fgetc(input);}
		// sau khi xuong dong thi kiem tra xem co dung la so tin luon khong, neu ko thi cu doc tiep
		while (!(kytuhientai>='0' && kytuhientai<='9') && kytuhientai!=EOF) {kytuhientai = fgetc(input);}
		// nam trong bang chu cai thì dau nhay lui lai 1 vi tri
		if (kytuhientai==EOF) break;
		// neu la so tin thi lui dau nhay lai de tiep tuc vong lap
		else fseek(input, -1, SEEK_CUR);
		// xuong dong doc tiep hoac la ket thuc file
		
		// if (kytuhientai==EOF) break;
	}
	// close file va tra lai con tro tro den danh sach
	fclose(input);
	return p_hocki;
}

// in ra lich su hoc
void InBangDiem(HK *p_HocKi) {
	int count =0;
	HK *p_hocki_temp = p_HocKi;
	HP *p_hocphan_temp;
	while (p_hocki_temp!=NULL) {
		printf("%d\n", p_hocki_temp->hocki);
		p_hocphan_temp = p_hocki_temp->hocphan;
		while (p_hocphan_temp!=NULL) {
			printf("%d\t%.1f\t%s\n", p_hocphan_temp->sotin, p_hocphan_temp->diemhp*0.1, p_hocphan_temp->mahp);
			p_hocphan_temp = p_hocphan_temp->next;
			count++;
		}
		p_hocki_temp = p_hocki_temp->next;
	}
	printf("So hoc phan: %d\n", count);
}

void GhiKetQua() {
	FILE *output = fopen("Thong Ke Ket Qua.txt", "w+");
	if (output==NULL) {
		printf("Khong mo duoc tep tin!\n");
		system("pause");
		exit(1);
	}

	HK *p_hocki = DocFileInput();

	HK *p_hocki_temp = p_hocki; 
	// tinh GPA CPA tat ca cac hoc ki
	fprintf(output, "Hoc ki\t\tGPA\t\tCPA\n");
	while (p_hocki_temp!=NULL) {
		//fprintf(output, "Hoc ki: %d\n", p_hocki_temp->hocki);
		TinhGPA(p_hocki, p_hocki_temp->hocki);
		TinhCPA(p_hocki, p_hocki_temp->hocki);
		fprintf(output, "%d\t\t%.2f\t\t%.2f\n",p_hocki_temp->hocki, p_hocki_temp->gpa, p_hocki_temp->cpa);
		p_hocki_temp = p_hocki_temp->next;
	}
	ThongKeTongHop(p_hocki);
	fprintf(output, "\nSo hoc phan da hoc: %d\n", sohocphandahoc);
	fprintf(output, "So tin da hoc: %d\n", sotindahoc);
	fprintf(output, "So tin da qua: %d\n", sotindaqua);
	fprintf(output, "So tin no: %d\n", sotinno);
	fprintf(output, "So hoc phan duoc A hoac A+: %d\n", sohocphanA);
	fprintf(output, "So hoc phan duoc B+: %d\n", sohocphanBp);
	fprintf(output, "So hoc phan duoc B: %d\n", sohocphanB);
	fprintf(output, "So hoc phan duoc C+: %d\n", sohocphanCp);
	fprintf(output, "So hoc phan duoc C: %d\n", sohocphanC);
	fprintf(output, "So hoc phan duoc D+: %d\n", sohocphanDp);
	fprintf(output, "So hoc phan duoc D: %d\n", sohocphanD);
	fprintf(output, "So hoc phan no: %d\n", sohocphanF);
	fclose(output);
}

int main() {
	GhiKetQua();
	return 0;
}