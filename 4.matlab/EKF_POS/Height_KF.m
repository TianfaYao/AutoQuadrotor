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
      if(isempty(X_LAST))
           X_LAST=single([0 0 0 0]');
      end
          
  persistent Z_LAST ;  
        if(isempty(Z_LAST))
           Z_LAST=single([0 0 0 0]');
        end
      
  persistent X_PRO_LAST ; %��һʱ�̺���
        if(isempty(X_PRO_LAST))
           X_PRO_LAST=single([0 0 0 0]');
        end
  
        
       X=F*X+F*(X-X_LAST);
       X_LAST=X;
       
       %�ſɱȾ���
%        F(1,1)=X(1)-X_PRO_LAST(1);
%        F(2,2)=X(2)-X_PRO_LAST(2);
%        F(3,3)=X(3)-X_PRO_LAST(3);
%        F(4,4)=X(4)-X_PRO_LAST(4);
%        %�ſɱȾ���
%        H(1,1)=Z(1)-Z_LAST(1);
%        H(2,3)=Z(2)-Z_LAST(2);
% X=F*X;
       
       P=F*P*F'+Q;
       K=P*H'/(H*P*H' +R); %����
       X=X+K*(Z-H*X);
       Z_LAST=Z;
       
       X_PRO_LAST=X;
       P=(single(I)-K*H)*P;
      
       h=X(1)
       v=X(2);
       a=X(3);