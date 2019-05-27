eHB_670 = 2.7951;
eHBO2_670 = 0.2940;
eH2O_670 = 0.0039355
eHB_850 = 0.691 
eHBO2_850 = 1.058
eH2O_850 = 0.043343
eHB_950 = 602.24/1000;
eHBO2_950 = 1204/1000;
eH2O_950 = 0.38759 %951

time = xlsread('cuff_test_mblldata.xlsx',1,'B:B');

test_670 = xlsread('cuff_test_mblldata.xlsx',1,'C:C')
test_850 = xlsread('cuff_test_mblldata.xlsx',1,'E:E');
test_950 = xlsread('cuff_test_mblldata.xlsx',1,'G:G');


base_rest_670 = 545.06; % 1x1
base_rest_850 = 627.7;
base_rest_950 = 471.285;
OD_670 = log10(base_rest_670 ./ test_670)';
OD_850 = log10(base_rest_850 ./ test_850)';
OD_950 = log10(base_rest_950 ./ test_950)';

OD = [OD_670;OD_850; OD_950]


% extinction coefficientsssss
lam1 = [eHB_670 eHBO2_670 eH2O_670];
lam2 = [eHB_850 eHBO2_850 eH2O_850];
lam3 = [eHB_950 eHBO2_950 eH2O_950];

e_matrix = [lam1; lam2; lam3];
inv_e_matrix = inv(e_matrix);
conc = inv_e_matrix * OD;

figure(1)
plot(time, conc(1,:), 'r-', 'linewidth', 1);
hold on
plot(time, conc(2,:), 'k-', 'linewidth', 1);
hold on
plot(time, conc(3,:), 'g-', 'linewidth', 1);
legend('Hb', 'HbO2', 'H2O')
xlabel('Time');
ylabel('Millimolar')

figure(2)
plot(time, test_670(:,1), 'r-', 'linewidth', 1);
hold on
plot(time, test_850(:,2), 'k-', 'linewidth', 1);
hold on
plot(time, test_950(:,3), 'g-', 'linewidth', 1);


[b,a] = butter(12,0.1,'low');           % IIR filter design zero phase
y = filtfilt(b,a,conc');    %apply to all data
figure; plot(conc');hold on; plot(y)  %show new figure

