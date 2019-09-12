% M-file,MSJGmethod.m
% 马斯京根法计算河道洪水演算
%%
% Q_m+1=C_0I_m+1+C_1I_m+C_2Q_m
% 其中C_0=(0.5t_-kx)/(0.5t_+k-kx);
% C_1=(0.5t_+kx)/(0.5t_+k-kx);
% C_2=(-0.5t_+k-kx)/(0.5t_+k-kx);
% C_0+C_1+C_2=1;
%%
% Q_m+1:第m+1时刻的出流
% Q_m:第m时刻的出流
% I_m+1:第m+1时刻的入流
% I_m:第m时刻的入流
% C_0:马斯京根流量演进系数
% C_1:马斯京根流量演进系数
% C_2:马斯京根流量演进系数
% t_:时段长
% k:蓄量流量关系曲线的坡度
% x:流量比重系数
%%
% Q_m=Q_m+1/C_2-C_0I_m+1/C_2-C_1I_m/C_2
% I_m=Q_m+1/C_1-C_0I_m+1/C_1-C_2Q_m/C_1
% I_m+1=Q_m+1/C_0-C_1I_m/C_0-C_2Q_m/C_0
%%
%长河段分为n个子河段，假定各个子河段的演进参数相等
%已知预报河段的k、x时，选定t_值，令k_t=t_，
%则单河段与多河段的演进参数的关系为：n=k/k_t=k/t_;x_t=1/2-n(1-2x)/2;

clear;clc;
% cd D:\matlab\R2016a\bin\LL_test2;
%read the file project.txt
FID=fopen('project.txt');
inppro=textscan(FID,'%f');
fclose(FID);
pro=inppro{1,1};
if pro==1%需要优化参数
    %read the file inputQm.txt ,the data struction: outflow
    FID=fopen('inputQm.txt');
    inputQm=textscan(FID,'%f');
    fclose(FID);
    Qobs=inputQm{1,1};
    
    %read the file paraUL.txt ,the data struction: paraName/paraMax/paraMin
    FID=fopen('paraUL.txt');
    paraUL=textscan(FID,'%s %f %f');
    fclose(FID);
    
    %read the file optiSL.txt ,the data struction: optiNumber;optiCriterion
    FID=fopen('optiSL.txt');
    optiSL=textscan(FID,'%s %f');
    fclose(FID);
    
    TNum=size(Qobs,1);
    Qav=mean(Qobs);
    Dif_oav=Qobs-Qav;
    
    %parameters limition
    paraL=paraUL{1,2};
    paraU=paraUL{1,3};
    
    %optimization method
    Num=optiSL{1,2}(1,1);
    Cri=optiSL{1,2}(2,1);
    Wei1=optiSL{1,2}(3,1);
    Wei2=optiSL{1,2}(4,1);
    Wei3=optiSL{1,2}(5,1);
    
    %generate parameters
    for n1=1:Num
        for n2=1:100
            CC0(n1,1)=paraL(1,1)+(paraU(1,1)-paraL(1,1))*rand;
            CC1(n1,1)=paraL(2,1)+(paraU(2,1)-paraL(2,1))*rand;
            CC2(n1,1)=1-CC0(n1,1)-CC1(n1,1);
            if CC2(n1,1)<0||CC2(n1,1)>1;
                continue
            else  break
            end
        end
    end
    IQ=zeros(TNum,Num);
    %read the file category.txt ,the data struction: category
    FID=fopen('category.txt');
    cate=textscan(FID,'%s %f');
    fclose(FID);
    NN=cate{1,2}(2,1);%the number of river subsections
    if cate{1,2}(1,1)==0 %main river
        %read the file inputIm.txt ,the data struction: inflow
        FID=fopen('inputIm.txt');%read the main river flow data
        inputIm=textscan(FID,'%f');
        fclose(FID);
        Iobs=inputIm{1,1};%sum inflow
    elseif cate{1,2}(1,1)==1 %main river and subriver
        %read the flow data,the data struction: inflow
        FID=fopen('inputIm.txt');%read the main river flow data
        inputIm=textscan(FID,'%f');
        fclose(FID);
        Iobs1=inputIm{1,1};
        FID=fopen('inputImadd.txt');%read the subriver flow data
        inputImadd=textscan(FID,'%f');
        fclose(FID);
        Iobs2=inputImadd{1,1};
%         Iobs=Iobs1+Iobs2;
        % calcuiate sum inflow
        [Mi1 indi1]=max(Iobs1);
        [Mi2 indi2]=max(Iobs2);
        if indi1==indi2
            Iobs=Iobs1+Iobs2;
        elseif indi1>indi2
            for n2=indi1-indi2+1:TNum
                Iobs2_(1:indi1-indi2,1)=Iobs2(1+indi1-indi2,1);
                Iobs2_(n2,1)=Iobs2(n2-(indi1-indi2),1);
            end
            Iobs=Iobs1+Iobs2_;
        else indi1<indi2
            for n2=indi2-indi1+1:TNum
                Iobs1_(1:indi1-indi2,1)=Iobs1(1+indi1-indi2,1);
                Iobs1_(n2,1)=Iobs1(n2-(indi2-indi1),1);
            end
            Iobs=Iobs1_+Iobs2;%sum inflow
        end
    end
    %calcuiate sum outflow
    for n1=1:Num
        IQ(:,n1)=Iobs;
    end
    for i=1:NN
        for n1=1:Num
            Qsim_m(1,n1)=Qobs(1); %mainriver flow initial value
            for n2=2:TNum
                Qsim_m(n2,n1)=CC0(n1,1)*IQ(n2,n1)+CC1(n1,1)*IQ(n2-1,n1)+CC2(n1,1)*Qsim_m(n2-1,1);
            end
        end
        IQ=Qsim_m;
    end
    for n1=1:Num
        Dif_om(:,n1)=Qobs(:,1)-Qsim_m(:,n1);
    end
    
    % code to calcuiate coefficients
    for n1=1:Num
        DC_m(n1,1)=1-sum(power(Dif_om(:,n1),2))/sum(power( Dif_oav(:,1),2));%Certainty coefficient确定性系数
        R=corrcoef(Qobs(:,1),Qsim_m(:,n1));
        R_m(n1,1)=R(1,2);%Correlation coefficient相关系数
        WB_m(n1,1)=1-abs(sum(Qobs)-sum(Qsim_m(:,n1)))/sum(Qobs);%Water balance coefficient水量平衡系数
        [m1,Ind1]=max(Qobs); [m2,Ind2]=max(Qsim_m(:,n1));
        P_m(n1,1)=m1-m2;%peak error峰值误差
        PT_m(n1,1)=Ind1-Ind2;%peak time error峰现时间误差
    end
    
    % code to get the optimal crirerion
    if (Cri==1)
        [DC,Ind]=max(Wei1*DC_m);
    elseif(Cri==2)
        [R,Ind]=max(Wei2*R_m);
    elseif(Cri==3)
        [WB,Ind]=max(Wei3*(1-WB_m));
    else(Cri==4)
        [COM,Ind]=max(Wei1*DC_m+Wei2*R_m+Wei3*(1-WB_m));
    end
    %Set the results
    para.C0=CC0(Ind,1);
    para.C1=CC1(Ind,1);
    para.C2=CC2(Ind,1);
    opti.DC=DC_m(Ind,1);
    opti.R=R_m(Ind,1);
    opti.WB=WB_m(Ind,1);
    opti.P=P_m(Ind,1);
    opti.PT=PT_m(Ind,1);
    
    k=(para.C1+para.C2)/(para.C0+para.C1)*1;
    x=(para.C0+para.C1)/(2*(para.C1+para.C2))+para.C0/(para.C0-1);
    
    FID=fopen('outputprec.txt','w');
    fprintf(FID,'确定性系数DC=%f\r',opti.DC);
    fprintf(FID,'\n');
    fprintf(FID,'相关系数R=%f\r',opti.R);
    fprintf(FID,'\n');
    fprintf(FID,'水量平衡系数WB=%f\r',opti.WB);
    fprintf(FID,'\n');
    fprintf(FID,'峰值误差P=%f\r',opti.P);
    fprintf(FID,'\n');
    fprintf(FID,'峰现时间误差TP=%f\r',opti.PT);
    fprintf(FID,'\n');
    fclose(FID);
    
    %output result 输出结果文件
    IQ=Iobs;
    for n1=1:NN
        for n2=2:TNum
            Qsim(1,1)=Qobs(1,1);
            Qsim(n2,1)=para.C0*IQ(n2,1)+para.C1*IQ(n2-1,1)+para.C2*Qsim(n2-1,1);
        end
        IQ=Qsim;
    end
    FID=fopen('outputQ.txt','w');
    for n2=1:TNum
        fprintf(FID,'%f\r',Qsim(n2,1));
        fprintf(FID,'\n');
    end
    fclose(FID);
    FID=fopen('para.txt','w');
    fprintf(FID,'C0  %f\r',para.C0);
    fprintf(FID,'\n');
    fprintf(FID,'C1  %f\r',para.C1);
    fprintf(FID,'\n');
    fprintf(FID,'C2  %f\r',para.C2);
    fprintf(FID,'\n');
    fclose(FID);
    
    figure;
    plot(Qobs,'b','linewidth',1);
    hold on;
    plot(Qsim,'r-o','linewidth',1);
    hold off;
    legend('Q实测','Q模拟','FontSize',12,'FontWeight','bold');
    legend('boxoff');
    xlabel('时段(h)');
    ylabel('流量(m^3/s)');
    title('20130602');
    ylmax1=max(Qsim);
    ylmax2=max(Qobs);
    ymaxs=[ylmax1 ylmax2];
    ylim([0 max(ymaxs)*1.2]);
    set(get(gca,'TITLE'),'FontSize',15);
    set(get(gca,'xlabel'),'FontSize',10,'FontWeight','bold');
    set(get(gca,'ylabel'),'FontSize',10,'FontWeight','bold');
    print(gcf,'-dpng','msjg.pnd');
    
elseif pro==0%已知参数，直接计算出流
    %read the file category.txt ,the data struction: category
    FID=fopen('category.txt');
    cate=textscan(FID,'%s %f');
    fclose(FID);
    NN=cate{1,2}(2,1);%the number of river subsections
    if cate{1,2}(1,1)==0 %main river
        %read the file inputIm.txt ,the data struction: inflow
        FID=fopen('inputIm.txt');%read the main river flow data
        inputIm=textscan(FID,'%f');
        fclose(FID);
        Iobs=inputIm{1,1};%sum inflow
        TNum=size(Iobs,1);
    elseif cate{1,2}(1,1)==1 %main river and subriver
        %read the flow data,the data struction: inflow
        FID=fopen('inputIm.txt');%read the main river flow data
        inputIm=textscan(FID,'%f');
        fclose(FID);
        Iobs1=inputIm{1,1};
        FID=fopen('inputImadd.txt');%read the subriver flow data
        inputImadd=textscan(FID,'%f');
        fclose(FID);
        Iobs2=inputImadd{1,1};
        TNum=max(size(Iobs1,1),size(Iobs1,1));
        % calcuiate sum inflow
        [Mi1 indi1]=max(Iobs1);
        [Mi2 indi2]=max(Iobs2);
        if indi1==indi2
            Iobs=Iobs1+Iobs2;
        elseif indi1>indi2
            for n2=indi1-indi2+1:TNum
                Iobs2_(1:indi1-indi2,1)=Iobs2(1+indi1-indi2,1);
                Iobs2_(n2,1)=Iobs2(n2-(indi1-indi2),1);
            end
            Iobs=Iobs1+Iobs2_;
        else indi1<indi2
            for n2=indi2-indi1+1:TNum
                Iobs1_(1:indi1-indi2,1)=Iobs1(1+indi1-indi2,1);
                Iobs1_(n2,1)=Iobs1(n2-(indi2-indi1),1);
            end
            Iobs=Iobs1_+Iobs2;%sum inflow
        end
    end
    %%  read the parameters value
    FID=fopen('para.txt');%read the main river flow data
    para=textscan(FID,'%s %f');
    fclose(FID);
    C0=para{1,2}(1,1);
    C1=para{1,2}(2,1);
    C2=para{1,2}(3,1);
    IQ=Iobs;
    for n1=1:NN
        for n2=2:TNum
            Qsim(1,1)=20;%flow initial value
            Qsim(n2,1)=C0*IQ(n2,1)+C1*IQ(n2-1,1)+C2*Qsim(n2-1,1);
        end
        IQ=Qsim;
    end
    FID=fopen('outputQ.txt','w');
    for n2=1:TNum
        fprintf(FID,'%f\r',Qsim(n2,1));
        fprintf(FID,'\n');
    end
    fclose(FID);
    figure;
    plot(Qsim,'r-o','linewidth',1);
    hold off;
    legend('Q模拟','FontSize',12,'FontWeight','bold');
    legend('boxoff');
    xlabel('时段(h)');
    ylabel('流量(m^3/s)');
    title('20130602');
    ymaxs=max(Qsim);
    ylim([0 max(ymaxs)*1.2]);
    set(get(gca,'TITLE'),'FontSize',15);
    set(get(gca,'xlabel'),'FontSize',10,'FontWeight','bold');
    set(get(gca,'ylabel'),'FontSize',10,'FontWeight','bold');
    print(gcf,'-dpng','msjg.pnd');
end





