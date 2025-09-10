#import<ios>
int i,n,a[1005],m=1e9+7;main(){scanf("%d",&n);for(*a=1;i<n*n+n;i++)(a[i%n+1]+=a[i%n])%=m;printf("%d %d",a[n],n*n);}
