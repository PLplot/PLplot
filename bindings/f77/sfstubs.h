C      common blocks required for routines in sfstubs.f
      integer maxlen
      parameter (maxlen = 320)
      character*(maxlen) string1, string2, string3
      character*(maxlen) string4, string5, string6
      character*(maxlen) string7, string8, string9
      integer s1(80), s2(80), s3(80),
     &  s4(80), s5(80), s6(80), s7(80), s8(80), s9(80)
      equivalence ( s1, string1 ), ( s2, string2 )
      equivalence ( s3, string3 ), ( s4, string4 )
      equivalence ( s5, string5 ), ( s6, string6 )
      equivalence ( s7, string7 ), ( s8, string8 )
      equivalence ( s9, string9 )
      common /zzplstr1/ string1
      common /zzplstr2/ string2
      common /zzplstr3/ string3
      common /zzplstr4/ string4
      common /zzplstr5/ string5
      common /zzplstr6/ string6
      common /zzplstr6/ string7
      common /zzplstr6/ string8
      common /zzplstr6/ string9
