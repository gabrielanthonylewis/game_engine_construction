del /s /q HAPI\*.*
rd HAPI
md HAPI
md HAPI\Licenses
md HAPI\Licenses\ffmpeg
md HAPI\Libs32
md HAPI\Libs64
copy ..\..\..\HAPI_2D_2016\HAPI\ HAPI\
copy ..\..\..\HAPI_2D_2016\HAPI\Licenses HAPI\Licenses
copy ..\..\..\HAPI_2D_2016\HAPI\Licenses\ffmpeg HAPI\Licenses\ffmpeg
copy ..\..\..\HAPI_2D_2016\HAPI\Libs32 HAPI\Libs32
copy ..\..\..\HAPI_2D_2016\HAPI\Libs64 HAPI\Libs64