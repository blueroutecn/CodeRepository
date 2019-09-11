#include"head.h"


using std::cout;
using std::endl;
using std::string;
using std::ofstream;


//#define __SOURCE
#ifdef __SOURCE
int main(int argc, char* argv[])
{
	extern void progress();
	//extern void output_Iprt(int, ofstream&);

	progress();


	system("pause");
	return 0;
}
#endif


/*   ��ˮ�����ڵ�ѹ��ˮ��ƽ���㷨��5.4�ڣ�����5.4��  */

// NP���ܹܶ�����NN���ܽڵ�����NHC��������֪ѹ������Ľڵ�������Iprt���м�������ָ������жϷ���
const int NP = 11, NN = 8, NHC = 7, Iprt = 1;

// ����м������
int i, j, kk1, k0, jb = 0;
double ac, ad, bc0, bc1, tcc, psign;

// ������۹�ʽ������
double baa = 200, bpc = 3135, alfa = 1.53;

// Hend�������ѹ��(m)�� maxDQ�Ǹ��ڵ���������պϲ ERDQ�ǽڵ������պϲ�ȡ�
double Hend = 37.5, maxDQ = 100.0, ERDQ = 0.001;

// qx�ǹܶ������� hx�ǹܵ���ˮ�� hf�ǹܶ�ˮ�� sp�ǹܶ�Ħ��ϵ����hf = sp*q2���� vpb�ǹܶ����١�
double qx[NP], hx[NP], hf[NP], sp[NP], vpb[NP];

// HF�ǽڵ�����ѹ���� DH�ǽڵ�У��ѹ���� DQ�ǽڵ������� sumc�ǽڵ�ѹ��У������Ԫ�أ� (d(hf)/d(q))^(-1) ����
double HF[NN], DH[NN], DQ[NN], sumc[NHC];

// I0��J0�ǹ����и��ܶε���ʼ�ڵ���ն˽ڵ��š�
int I0[NP] = { 0,1,2,4,1,2,3,5,6,0,4 };
int J0[NP] = { 1,2,3,3,5,6,7,6,7,1,3 };

// �ڵ���ˮ��(m3/s)��
double qj[NN] = { -0.19808, 0.0145, 0.05117, 0.0207, -0.03342, 0.03503, 0.08233, 0.02765 };

// �ڵ������(m)��
double Hj[NN] = { 13.6, 18.8, 19.1, 22.0, 32.2, 18.3, 17.3, 17.5 };

// �ڵ����ѹ��(m)��
double HH[NN] = { 46, 43, 40, 39, 40, 42, 39, 37.5 };

// �ܶγ���(m)��
double lp[NP] = { 320, 650, 550, 270, 330, 350, 360, 590, 490, 320, 270 };

// �ܶ�Ħ��Cֵ��
double HWC[NP] = { 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110 };

// �ܶα�׼�ܾ�(m)��
double Dpb[NP] = { 0.3, 0.3, 0.2, 0.2, 0.3, 0.2, 0.2, 0.3, 0.1, 0.3, 0.2 };




void progress()
{
	string s, ss;
	ofstream outfile;

	outfile.open("algrothmTest.txt");
	outfile << endl;
	outfile << "�ڵ�ѹ��ƽ����� �ܵ���� C= " << baa << "+" << bpc << "*pow(d," << alfa << ")" << endl;
	outfile << endl;

	outfile << "I0= ";
	for (i = 0; i < NP; i++)  outfile << "  " << I0[i];
	outfile << endl;
	outfile << "J0= ";
	for (i = 0; i < NP; i++)  outfile << "  " << J0[i];
	outfile << endl;

	outfile << "������������ ƽ����㿪ʼ ������������" << endl;
	outfile << "��׼�ܾ�Dpb[i]= ";
	for (i = 0; i < NP; i++) outfile << "  " << Dpb[i];
	outfile << endl;

	outfile << "���� ˮ��ƽ�� ����" << endl;
	outfile << "sp[i]= ";
	for (i = 0; i < NP; i++)
	{
		sp[i] = 10.67*lp[i] / (pow(HWC[i], 1.852)*pow(Dpb[i], 4.87));
		outfile << " " << sp[i];
	}outfile << endl; outfile << endl;

	outfile << "��ʼѹ��HH[i]= ";
	for (i = 0; i < NN; i++)  outfile << " " << HH[i];
	outfile << endl;

	for (i = 0; i < NP; i++)  qx[i] = 0.0;
	for (i = 0; i < NN; i++)
	{
		DQ[i] = qj[i];
		DH[i] = 0.0;
	}


	extern void output_Iprt(int, ofstream&);

	outfile << "���� ��ʼ������� ����" << endl;
	for (kk1 = 0; maxDQ > ERDQ; kk1++)
	{
		if (Iprt == 1) output_Iprt(kk1, outfile);  // �ж��Ƿ����������̡�

		maxDQ = 0.0;
		for (i = 0; i < NHC; i++)  // ��������δ֪ѹ���ڵ㡣
		{
			DQ[i] = qj[i];
			sumc[i] = 0.0;

			for (j = 0; j < NP; j++)  // �������йܶΡ�
			{
				if (I0[j] == i || J0[j] == i)  // �жϵ�ǰ�ܶ��Ƿ���ڵ�������
				{
					if (HH[I0[j]] == HH[J0[j]]) HH[J0[j]] = HH[J0[j]] + 0.00001*lp[j];  // ��֪����ʲô��

					qx[j] = pow(sp[j], -0.54)  // ���㵱ǰ�ܶ�������q = sp^(-1/n) * hf^(1/n)�� n=1.852��
							*pow(fabs(HH[I0[j]] - HH[J0[j]]), -0.46)*(HH[I0[j]] - HH[J0[j]]);

					hx[j] = HH[I0[j]] - HH[J0[j]];  // ���㵱ǰ�ܶ���ˮͷ��ʧ��
					hf[j] = sp[j] * pow(fabs(qx[j]), 0.852)*qx[j];  // ���㵱ǰ�ܶ��س�ˮ��, hf = sp * q^n, n=1.852��

					bc0 = 1.852*sp[j] * pow(fabs(qx[j]), 0.852);    // �����س�ˮ��������ĵ����� bc0 = d(hf)/d(q)��
					bc1 = pow(bc0, -1);
					sumc[i] += bc1;

					if (I0[j] == i)
					{
						k0 = J0[j];   // ��ǰ�ܶε���һ�˽ڵ㡣
						psign = 1.0;  // ���ڵ�ǰ�ܶζ��ڵ�ǰ�ڵ�ķ�������-1.0��ȥ��1.0����
					}
					if (J0[j] == i)
					{
						k0 = I0[j];
						psign = -1.0;
					}

					DQ[i] += psign * qx[j] - 0.5*bc1*DH[k0];  // ����ڵ�����������Ϊʲô������?
				}
			}

			DH[i] = -DQ[i] / sumc[i];  // ��ǰ�ڵ�У��ѹ����
			if (maxDQ < fabs(DQ[i])) maxDQ = fabs(DQ[i]);  // ������������պϲ
		}

		for (i = 0; i < NHC; i++) HH[i] += 0.5*DH[i];  // У���ڵ����ѹ����+0.5DH[i]������1.0DH[i]��Ϊ�����ȶ�����
	}

	outfile << "���� ����ܶ����� ����" << endl;
	for (i = 0; i < NP; i++)
	{
		ad = 0.7854*pow(Dpb[i], 2);
		vpb[i] = fabs(qx[i]) / ad;
	}
	outfile << "= = = = = =  ����ƽ�������ɣ� �ܶμ����� = = = = = = \n";
	outfile << "�ܺ�\t�Ͻڵ�\t�½ڵ�\t����\t�ܾ�\t����\tĦ��\t����\t��ʧ\n";
	outfile << "i\tI0\tJ0\tlp\tDpb\tqx\tHWC\tvpb\thf\t\n";
	for (i = 0; i < NP; i++)
	{
		outfile << i << "\t" << I0[i] << "\t" << J0[i] << lp[i] << "\t" << Dpb[i];
		outfile << "\t" << qx[i] << "\t" << HWC[i] << "\t" << vpb[i] << "\t" << hf[i] << "\t\n";
	}

	outfile << "= = = = = = = = =  �ڵ������  = = = = = = = = \n";
	outfile << "�ڵ��\t�ڵ�����\t����ѹ��\t�ڵ���\t����ѹ��\n";
	outfile << "i\tqj[i]\tHH[i]\tHj[i]\tHF[i]\n";
	for (i = 0; i < NN; i++) HF[i] = HH[i] - Hj[i];
	for (i = 0; i < NN; i++)
	{
		outfile << i << "\t" << qj[i] << "\t" << HH[i] << "\t" << Hj[i] << "\t" << HF[i] << "\n";
	}

	outfile << "= = = = = = = =  �������  = = = = = = = = =\n";
	tcc = 0.0;
	for (i = 0; i < NP; i++)
	{
		ac = baa + bpc * pow(Dpb[i], alfa)*lp[i];
		tcc += ac;
	}
	outfile << "== ������׼�ܾ�ƽ������ �ܵ������=  " << tcc << " Ԫ��==\n";

	outfile.close();
}


void output_Iprt(int kk1, ofstream& file)
{
	file << "kk1= " << kk1 << endl;

	file << "qx[NP]= ";
	for (int i = 0; i < NP; i++) file << std::setprecision(2) << qx[i];
	file << endl;

	file << "DQ[NN]= ";
	for (int i = 0; i < NN; i++) file << std::setprecision(2) << DQ[i];
	file << endl;

	file << "DH[NN]=";
	for (int i = 0; i < NN; i++)  file << std::setprecision(2) << DH[i];
	file << endl;

	file << "HH[NN]=";
	for (int i = 0; i < NN; i++)  file << std::setprecision(2) << HH[i];
	file << endl;

	file << "maxDQ= " << maxDQ << endl;
}






