#ifndef SPHERE_H
#define SPHERE_H

#include "Hittable.h"
#include "Vec3.h"

class Sphere : public Hittable {
    public:
        Sphere() {}
        Sphere(Vec3 cen, double r) : center(cen), radius(r) {};

        virtual bool Hit(
            const Ray& r, double t_min, double t_max, Hit_record& rec) const override;

    public:
        Vec3 center;
        double radius;
};

bool Sphere::Hit(const Ray& r, double t_min, double t_max, Hit_record& rec) const {
    Vec3 oc = r.origin() - center;
    auto a = r.direction().squaredLength();
    auto half_b = dot(oc, r.direction());
    auto c = oc.squaredLength() - radius*radius;

    auto discriminant = half_b*half_b - a*c;
    if (discriminant < 0) return false;
    auto sqrtd = sqrt(discriminant);

    // Find the nearest root that lies in the acceptable range.
    auto root = (-half_b - sqrtd) / a;
    if (root < t_min || t_max < root) {
        root = (-half_b + sqrtd) / a;
        if (root < t_min || t_max < root)
            return false;
    }

    rec.t = root;
    rec.p = r.at(rec.t);
    rec.normal = (rec.p - center) / radius;
    Vec3 outward_normal = (rec.p-center)/radius;
    rec.set_face_normal(r,outward_normal);

    return true;
}

#endif