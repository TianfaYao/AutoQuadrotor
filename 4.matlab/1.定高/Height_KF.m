function [ h v a ] = Height_KF(Q, H, R ,F, I ,Z )
%���� ytf
%���� 2017��4��27��
%״̬ת��Э�������
%H Ԥ�����
%R �۲���������
%F ״̬ת�ƾ���
%I
%Z ��������

persistent X ;% ״̬����
          if(isempty(X))
           X=single([0 0 0 0]');
          end
          
 persistent P ;% ״̬Э�������
            if(isempty(P))
            P=single([10 0 0 0;
                     0 10 0 0;
                     0 0 10 0;
                     0 0 0  100]);
            end
            
  persistent X_LAST ;     
            
%        X=F*X+F*(X-X_LAST);
%        X_LAST=X;
       X=F*X;
       P=F*P*F'+Q;
       K=P*H'/(H*P*H' +R); %����
       X=X+K*(Z-H*X);
       P=(single(I)-K*H)*P;
      
       h=X(1)
       v=X(2);
       a=X(3);