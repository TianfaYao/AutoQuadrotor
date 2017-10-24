function [ekf_zh ekf_zv ekf_za]=EKF_Attitude(Z)
%% �����ں��˲���
%״̬����
dt=0.02  ;
persistent X ;%��̬����
X=[0 0 0 0]';
%״̬ת�ƾ���
persistent H;%��̬����
H=[1 0 0 0;
   0 0 1 1];
%
persistent P;
P=[1 0 0 0;
    0 1 0 0;
    0 0 1 0;
    0 0 0  10];
persistent Q;
Q=[ 0.01    0       0        0;
    0         0.01   0        0 ;
    0         0       0.01   0;
    0         0       0        0.01];
persistent R;
R=[1 0;
   0 0.1];
persistent I;
I=eye(4);

 F=[1 dt 0 0;
    0 1 dt 0;
    0 0 1  0;
    0 0 0  1];
    %״ֵ̬
       X=F*X;
       P=F*P*F'+Q;
       %�۲�ֵ
       z=[Z(1);
          Z(2)];
       y=z-H*X;
       S=H*P*H' +R;
       K=P*H'*pinv(S);
       X=X+K*y;
       P=(I-K*H)*P;
ekf_zh=X(1);
ekf_zv=X(2);
ekf_za=X(3);
end