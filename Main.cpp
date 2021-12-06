#include<iostream>

#include "rtWeekend.h"

#include <fstream>
#include "Ray.h"
#include "Color.h"
#include "Vec3.h"
#include "Sphere.h"
#include "Hittable_list.h"
#include "Camera.h"

//Sphere 
double Hit_sphere(const Vec3& center, double radius, const Ray& r) {
    Vec3 oc = r.origin() - center;
    auto a = r.direction().squaredLength();
    auto half_b = dot(oc,r.direction());
    auto c = oc.squaredLength() - radius*radius;
    auto discriminant = half_b*half_b-a*c;
    if(discriminant<0)
    {
        return -1.0;
    } else {
        return(-half_b-sqrt(discriminant))/a;
    }
    //return (discriminant > 0);
}

Vec3 ray_color(const Ray& r,const Hittable& world,int depth)
{
    
    Hit_record rec;
    // If we've exceeded the ray bounce limit, no more light is gathered.
    if (depth <= 0)
    {
        return Vec3(0,0,0);
    }
    if (world.Hit(r, 0.001, infinity, rec)) 
    {
        Vec3 target = rec.p + rec.normal + random_unit_vector();
        return 0.5 * ray_color(Ray(rec.p, target - rec.p), world,depth-1);
    }
    Vec3 unit_direction = unitVector(r.direction());
    auto t = 0.5*(unit_direction.y() + 1.0);
    return (1.0-t)*Vec3(1.0, 1.0, 1.0) + t*Vec3(0.5, 0.7, 1.0);
}  

int main()
{
    std::ofstream output;
    output.open("output.ppm");
    
       // Image 

    const auto aspect_ratio = 16.0/9.0;
    const int image_width =400;
    const int image_height = static_cast<int>(image_width/aspect_ratio);
    const int samples_per_pixel = 100;
    const int max_depth = 50;

    // World
    Hittable_list world;
    world.add(make_shared<Sphere>(Vec3(0,0,-1), 0.5));
    world.add(make_shared<Sphere>(Vec3(0,-100.5,-1), 100));

    //Camera
        Camera cam;

    // Render

    output << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int j = image_height - 1; j >= 0; --j) {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i) {
                    Vec3 pixel_color(0, 0, 0);
            for (int s = 0; s < samples_per_pixel; ++s) {
                auto u = (i + random_double()) / (image_width-1);
                auto v = (j + random_double()) / (image_height-1);
                Ray r = cam.get_ray(u, v);
                pixel_color += ray_color(r, world,max_depth);
            }
            write_color(output, pixel_color, samples_per_pixel);
        
            
        }
    }

    std::cerr << "\nDone.\n";

 


}

