clc
clear
close all
%(['E:\RTSPHEM-main\vtk\react-data\lab_',num2str(m),'.phr'],'a'); 

nu = 0.008926;     % cm^2/s, kinematic viscosity
L = 0.005; %cm
% 指定要读取的txt文件路径
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
for n=1: 166
    file_path1 = ['I:\论文4\数据\1\test3...',num2str(n-1),'.csv'];%'I:\论文4\数据\1\test1.0.csv';
    file_path2 = ['I:\论文4\数据\1\test4.',num2str(n-1),'.csv'];

    % 调用 importdata() 函数读取txt文件内容并存储到变量 data 中
    data1 = importdata(file_path1);
    data2 = importdata(file_path2);
    %i=size(data.data,1);
    for i=1:(size(data1.data,1)-1)
        Vx(i) = (data1.data(i,1)+data1.data(i+1,1))/2;
        Vy(i) = (data1.data(i,2)+data1.data(i+1,2))/2;
        arc_length(i) = data1.data(i+1,6) - data1.data(i,6);
    end
    V(n) = sum((Vx'.^2+Vy'.^2).^(0.5).*arc_length')/sum(arc_length);% cm/s, out velosity
    delta_P(n) = mean(data2.data(:,4)) - mean(data1.data(:,4));
    k(n) = V(n)/delta_P(n)*nu*L;
end
k_row = find(k(:)>0);
k_row = k_row';
k_fix(:) = k(k(:)>0);
k_fix = k_fix';
k_fix_k0 = k_fix/k_fix(1);

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

file_path3 = 'I:\论文4\数据处理代码\ConsoleApplication4\ConsoleApplication4\test.txt';
data3 = importdata(file_path3);
data4 = sort(data3.data, 1);
data_0 = [0.62832, 1.258];              % 0时刻的体积和表面积
data4 = [data_0;data4];
pore = 1-data4(:,1)/(5*5*0.2);
vol = data4(:,1);
surf = data4(:,2);
sv = surf./vol;  %比表面积
k_kc1 = pore.^3./(1-pore).^2;
k_kc2 = pore.^3./(1-pore).^2./sv.^2;
k_kc1_k0 = k_kc1/k_kc1(1);
k_kc2_k0 = k_kc2/k_kc2(1);
pore_pore0 = pore/pore(1);