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


/*   给水管网节点压力水力平差算法（5.4节，例题5.4）  */

// NP是总管段数，NN是总节点数，NHC是流量已知压力代求的节点总数，Iprt是中间计算过程指针输出判断符。
const int NP = 11, NN = 8, NHC = 7, Iprt = 1;

// 相关中间变量。
int i, j, kk1, k0, jb = 0;
double ac, ad, bc0, bc1, tcc, psign;

// 管网造价公式参数。
double baa = 200, bpc = 3135, alfa = 1.53;

// Hend是最不利点压力(m)， maxDQ是各节点最大流量闭合差， ERDQ是节点流量闭合差精度。
double Hend = 37.5, maxDQ = 100.0, ERDQ = 0.001;

// qx是管段流量， hx是管道总水损， hf是管段水损， sp是管段摩阻系数（hf = sp*q2）， vpb是管段流速。
double qx[NP], hx[NP], hf[NP], sp[NP], vpb[NP];

// HF是节点自由压力， DH是节点校正压力， DQ是节点流量， sumc是节点压力校正矩阵元素（ (d(hf)/d(q))^(-1) ）。
double HF[NN], DH[NN], DQ[NN], sumc[NHC];

// I0，J0是管网中各管段的起始节点和终端节点编号。
int I0[NP] = { 0,1,2,4,1,2,3,5,6,0,4 };
int J0[NP] = { 1,2,3,3,5,6,7,6,7,1,3 };

// 节点用水量(m3/s)。
double qj[NN] = { -0.19808, 0.0145, 0.05117, 0.0207, -0.03342, 0.03503, 0.08233, 0.02765 };

// 节点地面标高(m)。
double Hj[NN] = { 13.6, 18.8, 19.1, 22.0, 32.2, 18.3, 17.3, 17.5 };

// 节点计算压力(m)。
double HH[NN] = { 46, 43, 40, 39, 40, 42, 39, 37.5 };

// 管段长度(m)。
double lp[NP] = { 320, 650, 550, 270, 330, 350, 360, 590, 490, 320, 270 };

// 管段摩阻C值。
double HWC[NP] = { 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110 };

// 管段标准管径(m)。
double Dpb[NP] = { 0.3, 0.3, 0.2, 0.2, 0.3, 0.2, 0.2, 0.3, 0.1, 0.3, 0.2 };




void progress()
{
	string s, ss;
	ofstream outfile;

	outfile.open("algrothmTest.txt");
	outfile << endl;
	outfile << "节点压力平差程序： 管道造价 C= " << baa << "+" << bpc << "*pow(d," << alfa << ")" << endl;
	outfile << endl;

	outfile << "I0= ";
	for (i = 0; i < NP; i++)  outfile << "  " << I0[i];
	outfile << endl;
	outfile << "J0= ";
	for (i = 0; i < NP; i++)  outfile << "  " << J0[i];
	outfile << endl;

	outfile << "—————— 平差计算开始 ——————" << endl;
	outfile << "标准管径Dpb[i]= ";
	for (i = 0; i < NP; i++) outfile << "  " << Dpb[i];
	outfile << endl;

	outfile << "—— 水力平差 ——" << endl;
	outfile << "sp[i]= ";
	for (i = 0; i < NP; i++)
	{
		sp[i] = 10.67*lp[i] / (pow(HWC[i], 1.852)*pow(Dpb[i], 4.87));
		outfile << " " << sp[i];
	}outfile << endl; outfile << endl;

	outfile << "初始压力HH[i]= ";
	for (i = 0; i < NN; i++)  outfile << " " << HH[i];
	outfile << endl;

	for (i = 0; i < NP; i++)  qx[i] = 0.0;
	for (i = 0; i < NN; i++)
	{
		DQ[i] = qj[i];
		DH[i] = 0.0;
	}


	extern void output_Iprt(int, ofstream&);

	outfile << "—— 开始迭代求解 ——" << endl;
	for (kk1 = 0; maxDQ > ERDQ; kk1++)
	{
		if (Iprt == 1) output_Iprt(kk1, outfile);  // 判断是否输出计算过程。

		maxDQ = 0.0;
		for (i = 0; i < NHC; i++)  // 遍历所有未知压力节点。
		{
			DQ[i] = qj[i];
			sumc[i] = 0.0;

			for (j = 0; j < NP; j++)  // 遍历所有管段。
			{
				if (I0[j] == i || J0[j] == i)  // 判断当前管段是否与节点相连。
				{
					if (HH[I0[j]] == HH[J0[j]]) HH[J0[j]] = HH[J0[j]] + 0.00001*lp[j];  // 不知道做什么？

					qx[j] = pow(sp[j], -0.54)  // 计算当前管段流量，q = sp^(-1/n) * hf^(1/n)， n=1.852。
							*pow(fabs(HH[I0[j]] - HH[J0[j]]), -0.46)*(HH[I0[j]] - HH[J0[j]]);

					hx[j] = HH[I0[j]] - HH[J0[j]];  // 计算当前管段总水头损失。
					hf[j] = sp[j] * pow(fabs(qx[j]), 0.852)*qx[j];  // 计算当前管段沿程水损, hf = sp * q^n, n=1.852。

					bc0 = 1.852*sp[j] * pow(fabs(qx[j]), 0.852);    // 计算沿程水损对流量的导数， bc0 = d(hf)/d(q)。
					bc1 = pow(bc0, -1);
					sumc[i] += bc1;

					if (I0[j] == i)
					{
						k0 = J0[j];   // 当前管段的另一端节点。
						psign = 1.0;  // 对于当前管段对于当前节点的方向（来流-1.0，去流1.0）。
					}
					if (J0[j] == i)
					{
						k0 = I0[j];
						psign = -1.0;
					}

					DQ[i] += psign * qx[j] - 0.5*bc1*DH[k0];  // 计算节点流量，但是为什么这样做?
				}
			}

			DH[i] = -DQ[i] / sumc[i];  // 当前节点校正压力。
			if (maxDQ < fabs(DQ[i])) maxDQ = fabs(DQ[i]);  // 调整最大流量闭合差。
		}

		for (i = 0; i < NHC; i++) HH[i] += 0.5*DH[i];  // 校正节点计算压力（+0.5DH[i]而不是1.0DH[i]是为计算稳定）。
	}

	outfile << "—— 计算管段流速 ——" << endl;
	for (i = 0; i < NP; i++)
	{
		ad = 0.7854*pow(Dpb[i], 2);
		vpb[i] = fabs(qx[i]) / ad;
	}
	outfile << "= = = = = =  管网平差计算完成： 管段计算结果 = = = = = = \n";
	outfile << "管号\t上节点\t下节点\t长度\t管径\t流量\t摩阻\t流速\t损失\n";
	outfile << "i\tI0\tJ0\tlp\tDpb\tqx\tHWC\tvpb\thf\t\n";
	for (i = 0; i < NP; i++)
	{
		outfile << i << "\t" << I0[i] << "\t" << J0[i] << lp[i] << "\t" << Dpb[i];
		outfile << "\t" << qx[i] << "\t" << HWC[i] << "\t" << vpb[i] << "\t" << hf[i] << "\t\n";
	}

	outfile << "= = = = = = = = =  节点计算结果  = = = = = = = = \n";
	outfile << "节点号\t节点流量\t计算压力\t节点标高\t自由压力\n";
	outfile << "i\tqj[i]\tHH[i]\tHj[i]\tHF[i]\n";
	for (i = 0; i < NN; i++) HF[i] = HH[i] - Hj[i];
	for (i = 0; i < NN; i++)
	{
		outfile << i << "\t" << qj[i] << "\t" << HH[i] << "\t" << Hj[i] << "\t" << HF[i] << "\n";
	}

	outfile << "= = = = = = = =  管网造价  = = = = = = = = =\n";
	tcc = 0.0;
	for (i = 0; i < NP; i++)
	{
		ac = baa + bpc * pow(Dpb[i], alfa)*lp[i];
		tcc += ac;
	}
	outfile << "== 管网标准管径平差结果： 管道总造价=  " << tcc << " 元。==\n";

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






