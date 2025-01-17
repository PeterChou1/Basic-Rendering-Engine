/*
  CSC D18 - Assignment 1 - 2D light propagation

  This is the place where you will be doing most of your work for solving this
  assignment. Before you start working here, you shold have become familiar
  with the data structures and functions available to you from light2D.h, and
  with the way a scene is built in buildScene.c

  Go over each of the functions below, and implement the different components
  of the solution in the sections marked

  /************************
  / TO DO:
  ************************ /

  Do not add or modify code outside these sections.

  Details about what needs to be implemented are described in the comments, as
  well as in the assignment handout. You must read both carefully. 

  Starter by: F.J. Estrada, Aug. 2017
*/

/****************************************************************************
 * Uncomment the #define below to enable debug code, add whatever you need
 * to help you debug your program between #ifdef - #endif blocks
 * ************************************************************************/
//#define __DEBUG_MODE

/*****************************************************************************
* COMPLETE THIS TEXT BOX:
*
* 1) Student Name:		Anson Feng
* 2) Student Name:		Chou Lu Wei
*
* 1) Student number:  1004721955
* 2) Student number:  1004295693
* 
* 1) UtorID           fengdian
* 2) UtorID           choulu1
* 
* We hereby certify that the work contained here is our own
*
* ___Anson Feng_______             _____Choulu1______
* (sign with your name)            (sign with your name)
********************************************************************************/

struct ray2D makeLightSourceRay(void)
{
 /*
   This function should return a light ray that has its origin at the light
   source, and whose direction depends on the type of light source.

   For point light sources (which emit light in all directions) the direction
    has to be chosen randomly and uniformly over a unit circle (i.e. any
    direction is equally likely)

   For a laser type light source, the direction of the ray is exactly the same
    as the direction of the lightsource.
    
   Set the colour of the ray to the same colour as the light source, and
    set the inside_outside flag to 0 (we assume light sources are 
    outside objects)

   In either case, the direction vector *must be unit length*.
*/
 
 /************************************************************************
 *  TO DO: Complete this function so that we can sample rays from the
 *         lightsource for the light propagation process.
 ************************************************************************/
 
struct ray2D ray = {0};

// This creates a dummy ray (which won't go anywhere since the direction
// vector d=[0 0]!. But it's here so you see which data values you have to
// provide values for given the light source position, and type.  
// ** REPLACE THE CODE BELOW with code that provides a valid ray that
//    is consistent with the lightsource.
 
ray.p.px=lightsource.l.p.px;			// Ray's origin(old)
ray.p.py=lightsource.l.p.py;     // Changed its origin to lightsource.location

if (lightsource.light_type) {    // Ray's direction: if laser, then we set direction as lightsource;
ray.d.px=lightsource.l.d.px;	// else, change direction to?
ray.d.py=lightsource.l.d.py;
} else {
double rand_ang=drand48()*2*PI;   // Using rand()/RAND_MAX to get number from [0, 1].
ray.d.px=cos(rand_ang);
ray.d.py=sin(rand_ang);
}
ray.inside_out=0;		// Initially 0 since the ray starts outside an object
ray.monochromatic=0;		// Initially 0 since the ray is white (from lightsource)
ray.R=lightsource.R;			// Ray colour in RGB must be the same as the lightsource
ray.G=lightsource.G;
ray.B=lightsource.B;
return(ray);			// Currently this returns dummy ray(old), now it should return correct ray
}

void propagateRay(struct ray2D *ray, int depth)
{
 /*
   This function carries out the light propagation process. It is provided with access
   to a ray data structure, and must perform the following steps (in order!):
   
   - Check if maximum recursion depth has been reached (in which case, it just returns)
   - Find the *closest* intersection between the ray and objects in the scene. This
     means you have to check against the 4 walls, and any circles added in buildScene,
     determine which intersection is closest, and obtain the intersection point, the
     normal at the intersection, and the lambda value at which the intersection happens.
   - Renders the ray onto the image from its starting point all the way up to the 
     intersection point.
   - At the intersection, use the material properties to determine how the propagation
     process proceeds:
         * For mirror materials, compute the mirror reflection direction, create a ray
           along that direction whose origin is the intersection point, and propagate
           that ray
         * For scattering materials, choose a random direction within +- 90 degrees of
           the normal, create a ray with that direction with origin at the intersection
           point, and propagate that ray
         * For refracting materials you will need to propagate two rays - one in the
           mirror reflection direction (just like for reflecting materials), and
           another in the refraction direction. Propagate both rays one after the other!
           
   NOTE: You should only care about intersections for which lambda is POSITIVE (in front
         of the ray), and greater than zero (e.g. if the ray is reflected from some
         object, you do not care about the intersection with the object itself which will
         have a lambda of *very close* to zero)
    
   In every case, make sure the ray's direction vector has unit length. You will need to
   complete other functions as part of your work here.
*/
  
 /*********************************************************************************
  * TO DO: Complete this function to implement light propagation!
  ********************************************************************************/
 
 // Define your local variables here
 
struct point2D intersectPt;
struct point2D normal;
int material_type = 0;
double refraction_index = 0.0;
double lambda_min = 0.0;
intersectPt.px=ray->p.px;
intersectPt.py=ray->p.py;

if (depth>=max_depth) return;	 	// Leave this be, it makes sure you don't recurse forever
if (ray->R <= 0.01 && ray->G <= 0.01 && ray->B <= 0.01) return;	// don't propagate a ray that has no colour
if (isnan(ray->R) || isnan(ray->G) || isnan(ray->B)) return;	// weird shit happens sometimes, this is a hack to avoid it


for (int i = 0; i < 4; i++) {
  if (i == 0 || i == 2) { // down/up-horizontal walls, which y is fixed.
    if (ray->d.py != 0) {
      double lambda = (walls[i].w.p.py - ray->p.py)/ray->d.py;
      struct point2D temp_normal_wall;
      temp_normal_wall.px = -walls[i].w.d.py;
      temp_normal_wall.py = walls[i].w.d.px;

      if ((lambda_min == 0.0 && lambda > 0.0) || (lambda < lambda_min && lambda > 0.0)) {
        if (lambda > 0.01 && ray->p.px + lambda*ray->d.px <= W_RIGHT && ray->p.px + lambda*ray->d.px >= W_LEFT) {
          lambda_min = lambda;  // only update when it is less than the current lambda_min, and ignore those tiny value, if all true, we update the intersectiPt
          intersectPt.px = ray->p.px + lambda_min*ray->d.px;
          intersectPt.py = ray->p.py + lambda_min*ray->d.py;
        }
        
        if (ray->p.px + lambda*ray->d.px <= W_RIGHT && ray->p.px + lambda*ray->d.px >= W_LEFT) { // Only update normal when intersect is not out of bound
          normal.px = temp_normal_wall.px;
          normal.py = temp_normal_wall.py;
          normalize(&normal);
        }

        material_type = walls[i].material_type;
      }
    }                     // Else: The Ray parallel to this line.
  } else if (i == 1 || i == 3) {                // left/right-vertical walls, which x is fixed.
    if (ray->d.px != 0) {
      double lambda = (walls[i].w.p.px - ray->p.px)/ray->d.px;
      struct point2D temp_normal_wall;
      temp_normal_wall.px = -walls[i].w.d.py;
      temp_normal_wall.py = walls[i].w.d.px;

      if ((lambda_min == 0.0 && lambda > 0.0) || (lambda < lambda_min && lambda > 0.0)) {
        if (lambda > 0.01 && ray->p.py + lambda*ray->d.py <= W_BOTTOM && ray->p.py + lambda*ray->d.py >= W_TOP) {
          lambda_min = lambda;  // only update when it is less than the current lambda_min and same as above.
          intersectPt.px = ray->p.px + lambda_min*ray->d.px;
          intersectPt.py = ray->p.py + lambda_min*ray->d.py;
        }

        if (ray->p.py + lambda*ray->d.py <= W_BOTTOM && ray->p.py + lambda*ray->d.py >= W_TOP) {
          normal.px = temp_normal_wall.px;
          normal.py = temp_normal_wall.py;
          normalize(&normal);
        }  

        material_type = walls[i].material_type;
      }
    }                     // Else: The Ray parallel to this line.
  }
}

intersectRay(ray, &intersectPt, &normal, &lambda_min, &material_type, &refraction_index);
renderRay(&ray->p, &intersectPt, ray->R, ray->G, ray->B);
if (ray->inside_out) {  
  normal.px = -normal.px;
  normal.py = -normal.py;  
}

if (material_type == 0) {
  struct ray2D refl_ray;
  refl_ray.p.px=intersectPt.px;
  refl_ray.p.py=intersectPt.py;
  refl_ray.R=ray->R;
  refl_ray.G=ray->G;
  refl_ray.B=ray->B;
  refl_ray.d.px=-2*dot(&normal, &ray->d)*normal.px+ray->d.px;
  refl_ray.d.py=-2*dot(&normal, &ray->d)*normal.py+ray->d.py;
  refl_ray.inside_out=ray->inside_out;
  refl_ray.H=ray->H;
  refl_ray.monochromatic=ray->monochromatic;
  propagateRay(&refl_ray, depth+1);
} else if (material_type == 1) {
  double rand_an = (drand48()-0.5)*PI;
  struct ray2D scat_ray;
  scat_ray.p.px=intersectPt.px;
  scat_ray.p.py=intersectPt.py;
  scat_ray.d.px=normal.px*cos(rand_an) - normal.py*sin(rand_an);
  scat_ray.d.py=normal.px*sin(rand_an) + normal.py*cos(rand_an);
  scat_ray.R=ray->R;
  scat_ray.G=ray->G;
  scat_ray.B=ray->B;
  scat_ray.H=ray->H;
  scat_ray.inside_out=ray->inside_out;
  scat_ray.monochromatic=ray->monochromatic;
  propagateRay(&scat_ray, depth+1);
} else if(material_type == 2) {
  struct point2D ref_norm = {-normal.px, -normal.py};
  struct point2D re_dir;
  struct ray2D refl_ray;
  struct ray2D transmitted_ray;
  // compute the schlick approximation
  double n1;
  double n2;
  re_dir.px = -ray->d.px;
  re_dir.py = -ray->d.py;
  // outside -> inside
  if (ray->inside_out == 0) {
    n1 = 1;
    n2 = refraction_index; 
  } else if (ray->inside_out == 1) {
  // inside -> outside
    n1 = refraction_index;
    n2 = 1;
  }
  // angle between the ray and normal (angle of incident)
  double ang_inc = acos(dot(&re_dir, &normal));
  // check theta less than 90
  double R0 = pow((n1 - n2) / (n1 + n2), 2);
  // strength of the reflected light
  double Rs =  R0 + ((1-R0) * pow(1 - cos(ang_inc), 5));
  // strength of the transmitted light
  double Rt = 1 - Rs;
  // reflected ray
  refl_ray.p.px=intersectPt.px;
  refl_ray.p.py=intersectPt.py;
  refl_ray.R= Rs * ray->R;
  refl_ray.G= Rs * ray->G;
  refl_ray.B= Rs * ray->B;
  refl_ray.d.px=-2*dot(&normal, &ray->d)*normal.px+ray->d.px;
  refl_ray.d.py=-2*dot(&normal, &ray->d)*normal.py+ray->d.py;
  refl_ray.inside_out= ray->inside_out;
  refl_ray.H=ray->H;
  refl_ray.monochromatic=ray->monochromatic;
  // the other is transmitted
  // transmitted ray
  transmitted_ray.p.px=intersectPt.px;
  transmitted_ray.p.py=intersectPt.py;
  transmitted_ray.R= Rt * ray->R;
  transmitted_ray.G= Rt * ray->G;
  transmitted_ray.B= Rt * ray->B;
  // find the angle of refraction
  double n =  ray->inside_out == 0 ? n1 / n2 : n2 / n1;
  double ang_ref = asin(n * sin(ang_inc));
  ang_ref =  ray->inside_out == 0 ? ang_ref : -ang_ref;
  //transmitted ray direction
  transmitted_ray.d.px = ref_norm.px*cos(ang_ref) - ref_norm.py*sin(ang_ref);
  transmitted_ray.d.py = ref_norm.px*sin(ang_ref) + ref_norm.py*cos(ang_ref);
  transmitted_ray.inside_out= (ray->inside_out == 0) ? 1 : 0;
  transmitted_ray.H=ray->H;
  transmitted_ray.monochromatic=ray->monochromatic; 
  //propagate the rays
  propagateRay(&refl_ray, depth+1);
  propagateRay(&transmitted_ray, depth+1);
}
}

void intersectRay(struct ray2D *ray, struct point2D *p, struct point2D *n, double *lambda, int *type, double *r_idx)
{
 /*
  This function checks for intersection between the ray and any objects in the objects 
  array. The objects are circles, so we are in fact solving for the intersection
  between a ray and a circle.
  
  For a unit circle centered at the origin, we would have the equation
  
  x^2 + y^2 = 1
  
  Using vector notation, with C=[x y]', we get
  
  ||C||^2 = 1
  
  A point on the ray is given by p + lambda*d
  
  Substituting in the equation for the circle we have 
  
  (p + lambda*d)(p + lambda*d) - 1 = 0
  
  If we expand the product above (here the product of two vectors is a DOT product), 
  we can form a quadratic equation
  
  A*lambda^2 + B*lambda + C = 0
  
  Which as you know, has a very simple solution. 
  
  Your task is to 
  * Figure out A, B, and C, considering that your circles don't necessarily have r=1
  * Figure out how to deal with the fact that circles in the scene are likely
    *not* centered at the origin
    
  Then implement the code that will find the value(s) of lambda at the intersection(s).
  
  Note that you can have no intersections, 1 intersection, or 2 intersections
  
  This function *must* find the closest intersection (if any) and update the value
  of lambda, the intersection point p, the normal n at the intersection, 
  the corresponding object's material type (needed outside here to figure out how
  to handle the light's bouncing off this object), and the index of refraction for
  the object (needed if this is a transparent object). 
  
  You must make sure that n is a unit-length vector.
 */
 
 /**********************************************************************************
  * TO DO: Complete this function to find the closest intersection between the
  *        ray and any objects in the scene, as well as the values at the
  *        intersection that will be needed to determine how to bounce/refract the
  *	   ray.
  * *******************************************************************************/
for (int i = 0; i < MAX_OBJECTS; i++) {
    struct circ2D* c = &objects[i];
    if (c->r ==  -1) {
      continue;
    }
    double A = dot(&ray->d, &ray->d);
    double B =  2 * dot(&ray->d, &ray->p) - 2 * dot(&ray->d, &c->c);
    double C = dot(&c->c, &c->c) - 2 * dot(&ray->p, &c->c) + dot(&ray->p, &ray->p) - pow(c->r,2);
    // solve lambda using quadratic equation
    double discrim = pow(B, 2) - 4 * A * C;
    if (discrim < 0) {
        continue;
    }
    double lambda1 = (-B + sqrt(discrim)) / (2 * A);
    double lambda2 = (-B - sqrt(discrim)) / (2 * A);
    double temp;
    if (lambda1 > 0.001 && lambda2 > 0.001) {
        temp = (lambda1 <= lambda2) ? lambda1 : lambda2;
    } else if (lambda1 > 0.001) {
        temp = lambda1;
    } else if (lambda2 > 0.001) {
        temp = lambda2;
    } else {
        continue;
    }
    // check if lambda is positive and smaller than current lambda
    if (temp > 0.0 && temp < *lambda) {
      p->px = ray->p.px;  // make sure the ray doesn't intersect the wall as we have a smaller lambda.
      p->py = ray->p.py;
      if (temp > 0.001) {
        // update lambda
        *lambda = temp;
        // update intersection point
        p->px = ray->p.px + temp * ray->d.px;
        p->py = ray->p.py + temp * ray->d.py;
      }
      // update normal
      n->px = p->px - c->c.px;
      n->py = p->py - c->c.py;
      normalize(n);
        // update material type
      *type = c->material_type;
        // update index of refraction
      *r_idx = c->r_idx;
    }
}
}
