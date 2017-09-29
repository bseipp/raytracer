raytrace: Tracer.o Ray.o Sphere.o sVector.o Fill.o HitRecord.o
	g++ -Wall -o raytrace Tracer.o Ray.o Sphere.o sVector.o Fill.o HitRecord.o

Tracer.o: Tracer.cpp Ray.h Sphere.h Fill.h sVector.h HitRecord.h
	g++ -Wall -ansi -c Tracer.cpp

Fill.o: Fill.cpp Fill.h sVector.h
	g++ -Wall -ansi -c Fill.cpp

HitRecord.o: HitRecord.cpp HitRecord.h sVector.h Sphere.h
	g++ -Wall -ansi -c HitRecord.cpp

Ray.o: Ray.cpp Ray.h sVector.h
	g++ -Wall -ansi -c Ray.cpp

Sphere.o: Sphere.cpp Sphere.h Fill.h
	g++ -Wall -ansi -c Sphere.cpp

sVector.o: sVector.cpp sVector.h 
	g++ -Wall -ansi -c sVector.cpp

run: raytrace
	./raytrace balls-3.nff

clean:
	rm raytrace *.o trace.ppm