                            >  {     �  )   	  �    #   File:       modregLib.make#   Target:     modregLib#   Created:    Jago, Jul 2001, Stefano M. Iacus##   This script assumes that R binary already exists in #   folder 'src/macintosh/bin'##   Tested with:##   MPW Shell 3.6d7#   MrC C Compiler 5.0.0d3c1#   Universal Headers 3.4#   CarbonLib 1.4#DLib            = modregMAKEFILE        = {DLib}Lib.make�MondoBuild�    = {MAKEFILE}  # Make blank to avoid rebuilds when makefile is modifiedMacF2C     		=  "::::macintosh:Mac F2C"F2CDir          =  ::::macintosh:f2c:Includes        = -i ::::include: �				  -i ::::macintosh: �                  -i {F2CDir}				  Sym-PPC         = -sym offPPCCOptions     = {Includes} {Sym-PPC} -opt off -includes unix -w 35,2 -shared_lib_export on -d HAVE_CONFIG_H -d Macintosh �                  -d TARGET_API_MAC_CARBON=1 -prefix RHeaders.h -align power 				  ### Library directory ###LibDir     		= ::::macintosh:bin:library:{DLib}:libs### Source Files ### SrcFiles        =  	bsplvd.c �					bvalue.c �					bvalus.c �					init.c �					ksmooth.c �					loessc.c �					loessf.c �					ppr.c �					qsbart.c �					sbart.c �					sgram.c �					sinerp.c �					sslvrg.c �					stxwx.c �					xxxsbart_com.c �					::::main:xxxpr.c### Object Files ###ObjFiles-PPC    =   bsplvd.o �					bvalue.o �					bvalus.o �					init.o �					ksmooth.o �					loessc.o �					loessf.o �					ppr.o �					qsbart.o �					sbart.o �					sgram.o �					sinerp.o �					sslvrg.o �					stxwx.o �                    ::::main:xxxpr.o �                    xxxsbart_com.oFortFiles		=	bsplvd.f �					bvalue.f �					bvalus.f �					loessf.f �					ppr.f �					qsbart.f �					sbart.f �					sgram.f �					sinerp.f �					sslvrg.f �					stxwx.f �					::::main:xxxpr.f### Libraries ###LibFiles-PPC    =  �				  "{SharedLibraries}CarbonLib" �				  "{PPCLibraries}PPCCRuntime.o" �				  "{SharedLibraries}StdCLib" �				  "::::macintosh:bin:R"  �				  "{F2CDir}f2cLib"### Default Rules ### .o  �  .c  {�MondoBuild�}	{PPCC} {depDir}{default}.c -o {targDir}{default}.o {PPCCOptions}#.c  �  .f  {�MondoBuild�}#	 {MacF2C} -wait {depDir}{default}.f 	### Build Rules ###{DLib}Lib  ��  {ObjFiles-PPC} {LibFiles-PPC} {�MondoBuild�} #create export table    if `Exists :expvar`	 delete :expvar	end    catenate �.x > expvar# checks if modules directory exsists    if ! `Exists -d "{LibDir}"`	 echo "Creating libs directory" "{LibDir}"      NewFolder "{LibDir}"    end# Build the vfonts module		PPCLink �		-o {LibDir}:{DLib}Lib �		{ObjFiles-PPC} �		{LibFiles-PPC} �		{Sym-PPC} �		-mf -d �		-t 'shlb' �		-c '????' �		-xm s �		-@export expvar# Removing mass    Delete   :�.x	Delete   :�.o     Delete   :expvar	### Required Dependencies ### init.o � init.cksmooth.o �  ksmooth.cloessc.o � loessc.cbsplvd.o � bsplvd.cbvalue.o � bvalue.cbvalus.o � bvalus.cloessf.o � loessf.cppr.o    � ppr.cqsbart.o � qsbart.csbart.o  � sbart.csgram.o  � sgram.csinerp.o � sinerp.csslvrg.o � sslvrg.cstxwx.o  � stxwx.cxxsbart_com.o � xxxsbart_com.c::::main:xxxpr.o � ::::main:xxxpr.c# f2c dependenciesbsplvd.c � bsplvd.f	 {MacF2C} -wait {FortFiles} bvalue.c � bvalue.fbvalus.c � bvalus.floessf.c � loessf.fppr.c    � ppr.fqsbart.c � qsbart.fsbart.c  � sbart.fsgram.c  � sgram.fsinerp.c � sinerp.fsslvrg.c � sslvrg.fstxwx.c  � stxwx.fxxxsbart_com.c � sbart.f::::main:xxxpr.c � ::::main:xxxpr.f### Optional Dependencies ###### Build this target to generate "include file" dependencies. ###Dependencies  �  $OutOfDate	MakeDepend �		-append {MAKEFILE} �		-ignore "{CIncludes}" �		-objdir ":" �		-objext .o �		{Includes} �		{SrcFiles}     �   �   a                                                                                                                                                                                                                                                   J 	Monaco    4w�                     nJ�, nJ�,����  
�  
�  � ��    nJ�, nJ�,  �             TB-1�?�20         ��H����                iacus   modregLib.make    mpw makefile      �   �   aSORTf �  R MPSR  ckid   *���        ���   N     �     p��Projector DataTEXTMPS  ����                  