#include "physics.h"
#include "scene.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdio.h>
#include <GL/gl.h>

static void near_callback(void* data, dGeomID o1, dGeomID o2) {
    PhysicsWorld* pw = (PhysicsWorld*)data;
    dBodyID b1 = dGeomGetBody(o1);
    dBodyID b2 = dGeomGetBody(o2);

    if (b1 && b2 && dAreConnectedExcluding(b1, b2, dJointTypeContact)) {
        return;
    }

    dContact contact[MAX_CONTACTS];
    for (int i = 0; i < MAX_CONTACTS; ++i) {
        contact[i].surface.mode = dContactApprox1;
        contact[i].surface.mu = pw->surface_friction;
        contact[i].surface.bounce = pw->bounce;
        contact[i].surface.bounce_vel = 0.1;
        contact[i].surface.soft_erp = pw->erp;
        contact[i].surface.soft_cfm = pw->cfm;
    }

    int n = dCollide(o1, o2, MAX_CONTACTS, &contact[0].geom, sizeof(dContact));
    for (int i = 0; i < n; ++i) {
        if (!isfinite(contact[i].geom.pos[0]) || !isfinite(contact[i].geom.pos[1]) || !isfinite(contact[i].geom.pos[2])) {
            continue;
        }

        dJointID c = dJointCreateContact(pw->world, pw->contact_group, &contact[i]);
        dJointAttach(c, b1, b2);

        Object* obj1 = b1 ? (Object*)dBodyGetData(b1) : NULL;
        Object* obj2 = b2 ? (Object*)dBodyGetData(b2) : NULL;
        if (obj1 && !obj1->is_static && obj1->is_active) {
            Vec3 v1, v2, dv;
            physics_get_linear_velocity(&obj1->physics_body, &v1);
            
            if (b2) {
                physics_get_linear_velocity(&obj2->physics_body, &v2);
            }
            else {
                v2 = (Vec3){0,0,0};
            }

            dv = vec3_substract(v1, v2);
            float speed = vec3_length(dv);
            float damage = speed * 0.1f;  
            obj1->value -= damage;

            if (obj1->value <= 0.0f) {
                obj1->is_active = false;
                
                dGeomDestroy(obj1->physics_body.geom);
                dBodyDestroy(obj1->physics_body.body);
            }
        }
    }
}

void init_physics(PhysicsWorld* pw) {
    dInitODE2(0);

    pw->world = dWorldCreate();
    pw->space = dHashSpaceCreate(NULL);
    pw->contact_group = dJointGroupCreate(0);

    PhysicsConfig cfg = physics_default_config();
    physics_apply_config(pw, &cfg);
}

void physics_init_gravity(PhysicsWorld* pw, const dReal gravity[3]) {
    memcpy(pw->gravity, gravity, 3 * sizeof(dReal));
    dWorldSetGravity(pw->world, gravity[0], gravity[1], gravity[2]);
}

PhysicsConfig physics_default_config() {
    PhysicsConfig cfg = {
        .max_correcting_vel = 0.9,
        .erp = 0.2,
        .cfm = 1e-5f,
        .linear_damping = 0.01,
        .angular_damping = 0.05,
        .surface_friction = 0.7,
        .bounce = 0.1,
        .auto_disable_linear_threshold = 0.01,
        .auto_disable_angular_threshold = 0.01,
        .auto_disable_steps = 10,
        .quick_step_iterations = 50
    };
    return cfg;
}

void physics_apply_config(PhysicsWorld* pw, PhysicsConfig* cfg) {
    if (!pw || !cfg) return;
    pw->erp = cfg->erp;
    pw->cfm = cfg->cfm;
    pw->surface_friction = cfg->surface_friction;
    pw->bounce = cfg->bounce;

    dWorldSetERP(pw->world, cfg->erp);
    dWorldSetCFM(pw->world, cfg->cfm);
    dWorldSetContactMaxCorrectingVel(pw->world, cfg->max_correcting_vel);
    dWorldSetLinearDamping(pw->world, cfg->linear_damping);
    dWorldSetAngularDamping(pw->world, cfg->angular_damping);

    dWorldSetAutoDisableFlag(pw->world, 1);
    dWorldSetAutoDisableLinearThreshold(pw->world, cfg->auto_disable_linear_threshold);
    dWorldSetAutoDisableAngularThreshold(pw->world, cfg->auto_disable_angular_threshold);
    dWorldSetAutoDisableSteps(pw->world, cfg->auto_disable_steps);

    dWorldSetQuickStepNumIterations(pw->world, cfg->quick_step_iterations);
}

void physics_create_box(PhysicsWorld* pw, PhysicsBody* pb, double mass, Vec3 pos, Vec3 half_extents) {
    pb->body = dBodyCreate(pw->world);
    pb->dimensions = half_extents;
    dMass m;
    dMassSetZero(&m);
    dMassSetBoxTotal(&m, mass > 0 ? mass : 1.0,
        half_extents.x * 2,
        half_extents.y * 2,
        half_extents.z * 2);
    dBodySetMass(pb->body, &m);
    dBodySetPosition(pb->body, pos.x, pos.y, pos.z);
    
    pb->geom = dCreateBox(pw->space,
        half_extents.x * 2,
        half_extents.y * 2,
        half_extents.z * 2);
    dGeomSetBody(pb->geom, pb->body);
    pb->is_active = true;
}

void physics_create_wall_filled(PhysicsWorld* pw, Vec3 center, Vec3 dim, Direction dir, float thickness) {
    float w = dim.x * 0.5f;
    float l = dim.y * 0.5f;
    float h = dim.z;

    float dx = 0.1f;
    float dy = 0.1f;
    float dz = 0.1f;;
    Vec3 pos = center;

    if (dir == DIR_NORTH) {
        dx = dim.x;
        dy = thickness;
        dz = h;
        pos.y += l + (thickness * 0.5f);
        pos.z += h * 0.5f;
    }
    if (dir == DIR_SOUTH) {
        dx = dim.x;
        dy = thickness;
        dz = h;
        pos.y -= (l + thickness * 0.5f);
        pos.z += h * 0.5f;
    }
    if (dir == DIR_EAST) {
        dx = thickness;
        dy = dim.y;
        dz = h;
        pos.x += (w + thickness * 0.5f);
        pos.z += h * 0.5f;
    }
    if (dir == DIR_WEST) {
        dx = thickness;
        dy = dim.y;
        dz = h;
        pos.x -= (w + thickness * 0.5f);
        pos.z += h * 0.5f;
    }

    dGeomID box = dCreateBox(pw->space, dx, dy, dz);
    dGeomSetPosition(box, pos.x, pos.y, pos.z);
}

void physics_create_wall_connector(PhysicsWorld* pw, Vec3 center, Vec3 dim, Direction dir, float wall_thickness, float door_width, float door_height) {
    Vec3 pos;
    dGeomID geom;
    float wall_x, wall_y;
    float w = dim.x * 0.5f;
    float l = dim.y * 0.5f;
    float h = dim.z;
    float jamb_w = (dim.x - door_width) * 0.5f;
    float jamb_l = (dim.y - door_width) * 0.5f;
    float t2 = wall_thickness * 0.5f;

    door_width = fmaxf(door_width, 0.01f);
    wall_thickness = fmaxf(wall_thickness, 0.01f);
    door_height = fmaxf(door_height, 0.01f);

    if (dir == DIR_NORTH) {
        wall_y = center.y + l + t2;

        pos = (Vec3){ center.x, wall_y, center.z + door_height + (h - door_height) * 0.5f };
        geom = dCreateBox(pw->space, door_width, wall_thickness, (h - door_height));
        dGeomSetPosition(geom, pos.x, pos.y, pos.z);

        pos = (Vec3){ center.x - (w - jamb_w * 0.5f), wall_y, center.z + door_height * 0.5f };
        geom = dCreateBox(pw->space, jamb_w, wall_thickness, door_height);
        dGeomSetPosition(geom, pos.x, pos.y, pos.z);

        pos = (Vec3){ center.x + (w - jamb_w * 0.5f), wall_y, center.z + door_height * 0.5f };
        geom = dCreateBox(pw->space, jamb_w, wall_thickness, door_height);
        dGeomSetPosition(geom, pos.x, pos.y, pos.z);
    }

    if (dir == DIR_EAST) {
        wall_x = center.x + w + t2;
        
        pos = (Vec3){ wall_x, center.y, center.z + door_height + (h - door_height) * 0.5f };
        geom = dCreateBox(pw->space, wall_thickness, door_width, (h - door_height));
        dGeomSetPosition(geom, pos.x, pos.y, pos.z);

        pos = (Vec3){ wall_x, center.y - (l - jamb_l * 0.5f), center.z + door_height * 0.5f };
        geom = dCreateBox(pw->space, wall_thickness, jamb_l, door_height);
        dGeomSetPosition(geom, pos.x, pos.y, pos.z);

        pos = (Vec3){ wall_x, center.y + (l - jamb_l * 0.5f), center.z + door_height * 0.5f };
        geom = dCreateBox(pw->space, wall_thickness, jamb_l, door_height);
        dGeomSetPosition(geom, pos.x, pos.y, pos.z);
    }

    if (dir == DIR_SOUTH) {
        wall_y = center.y - l - t2;

        pos = (Vec3){ center.x, wall_y, center.z + door_height + (h - door_height) * 0.5f };
        geom = dCreateBox(pw->space, door_width, wall_thickness, (h - door_height));
        dGeomSetPosition(geom, pos.x, pos.y, pos.z);

        pos = (Vec3){ center.x - (w - jamb_w * 0.5f), wall_y, center.z + door_height * 0.5f};
        geom = dCreateBox(pw->space, jamb_w, wall_thickness, door_height);
        dGeomSetPosition(geom, pos.x, pos.y, pos.z);
        
        pos = (Vec3){center.x + (w - jamb_w * 0.5f), wall_y, center.z + door_height * 0.5f};
        geom = dCreateBox(pw->space, jamb_w, wall_thickness, door_height);
        dGeomSetPosition(geom, pos.x, pos.y, pos.z);
    }

    if (dir == DIR_WEST) {
        wall_x = center.x - w - t2;

        pos = (Vec3){ wall_x, center.y, center.z + door_height + (h - door_height) * 0.5f };
        geom = dCreateBox(pw->space, wall_thickness, door_width, (h - door_height));
        dGeomSetPosition(geom, pos.x, pos.y, pos.z);

        pos = (Vec3){wall_x, center.y - (l - jamb_l * 0.5f), center.z + door_height * 0.5f};
        geom = dCreateBox(pw->space, wall_thickness, jamb_l, door_height);
        dGeomSetPosition(geom, pos.x, pos.y, pos.z);
        
        pos = (Vec3){wall_x, center.y + (l - jamb_l * 0.5f), center.z + door_height * 0.5f};
        geom = dCreateBox(pw->space, wall_thickness, jamb_l, door_height);
        dGeomSetPosition(geom, pos.x, pos.y, pos.z);
    }
}

void physics_create_ground_plane(PhysicsWorld* pw) {
    if (!pw) return;
    dCreatePlane(pw->space, 0, 0, 1, 0);
}

void physics_simulate(PhysicsWorld* pw, double elapsed) {
    if (!pw) return;
    const double maxStep = 1.0 / 30.0;
    double step = (elapsed > maxStep ? maxStep : elapsed);
    int substeps = 2;
    double subdt = step / substeps;

    for (int i = 0; i < substeps; ++i) {
        dSpaceCollide(pw->space, pw, &near_callback);
        dWorldQuickStep(pw->world, subdt);
        dJointGroupEmpty(pw->contact_group);
    }
}

static void quat_to_mat3(const dReal q[4], double M[3][3]) {
    double w = q[0], x = q[1], y = q[2], z = q[3];
    M[0][0] = 1 - 2 * y * y - 2 * z * z;
    M[0][1] = 2 * x * y - 2 * z * w;
    M[0][2] = 2 * x * z + 2 * y * w;
    M[1][0] = 2 * x * y + 2 * z * w;
    M[1][1] = 1 - 2 * x * x - 2 * z * z;
    M[1][2] = 2 * y * z - 2 * x * w;
    M[2][0] = 2 * x * z - 2 * y * w;
    M[2][1] = 2 * y * z + 2 * x * w;
    M[2][2] = 1 - 2 * x * x - 2 * y * y;
}

bool physics_get_obb_corners(PhysicsBody* pb, Vec3 out_corners[8]) {
    if (!pb || !pb->body) return false;
    const dReal* p = dBodyGetPosition(pb->body);
    const dReal* q = dBodyGetQuaternion(pb->body);
    Vec3 c = { (float)p[0], (float)p[1], (float)p[2] };
    Vec3 e = pb->dimensions;

    double M[3][3];
    quat_to_mat3(q, M);

    Vec3 L[8] = {
        { e.x,  e.y,  e.z}, {-e.x,  e.y,  e.z},
        {-e.x, -e.y,  e.z}, { e.x, -e.y,  e.z},
        { e.x,  e.y, -e.z}, {-e.x,  e.y, -e.z},
        {-e.x, -e.y, -e.z}, { e.x, -e.y, -e.z}
    };

    for (int i = 0; i < 8; ++i) {
        Vec3 l = L[i];
        out_corners[i].x = c.x + (float)(M[0][0] * l.x + M[0][1] * l.y + M[0][2] * l.z);
        out_corners[i].y = c.y + (float)(M[1][0] * l.x + M[1][1] * l.y + M[1][2] * l.z);
        out_corners[i].z = c.z + (float)(M[2][0] * l.x + M[2][1] * l.y + M[2][2] * l.z);
    }
    return true;
}

bool ray_intersect_obb(Vec3 ray_o, Vec3 ray_d, PhysicsBody* pb, float* tmin_out) {
    if (!pb || !pb->body) return false;
    const dReal* p = dBodyGetPosition(pb->body);
    const dReal* q = dBodyGetQuaternion(pb->body);
    Vec3 c = { (float)p[0], (float)p[1], (float)p[2] };
    Vec3 e = pb->dimensions;

    double M[3][3];
    quat_to_mat3(q, M);
    double Mt[3][3] = {
        { M[0][0], M[1][0], M[2][0] },
        { M[0][1], M[1][1], M[2][1] },
        { M[0][2], M[1][2], M[2][2] }
    };

    Vec3 p0 = { ray_o.x - c.x, ray_o.y - c.y, ray_o.z - c.z };
    Vec3 o_local = {
        (float)(Mt[0][0] * p0.x + Mt[0][1] * p0.y + Mt[0][2] * p0.z),
        (float)(Mt[1][0] * p0.x + Mt[1][1] * p0.y + Mt[1][2] * p0.z),
        (float)(Mt[2][0] * p0.x + Mt[2][1] * p0.y + Mt[2][2] * p0.z)
    };
    Vec3 d_local = {
        (float)(Mt[0][0] * ray_d.x + Mt[0][1] * ray_d.y + Mt[0][2] * ray_d.z),
        (float)(Mt[1][0] * ray_d.x + Mt[1][1] * ray_d.y + Mt[1][2] * ray_d.z),
        (float)(Mt[2][0] * ray_d.x + Mt[2][1] * ray_d.y + Mt[2][2] * ray_d.z)
    };

    float tmin = 0.0f, tmax = FLT_MAX;
    if (!slab_hit(o_local.x, d_local.x, -e.x, e.x, &tmin, &tmax)) return false;
    if (!slab_hit(o_local.y, d_local.y, -e.y, e.y, &tmin, &tmax)) return false;
    if (!slab_hit(o_local.z, d_local.z, -e.z, e.z, &tmin, &tmax)) return false;

    *tmin_out = tmin;
    return true;
}

void physics_get_position(PhysicsBody* pb, Vec3* position) {
    if (!pb || !pb->body || !position) return;
    const dReal* pos = dBodyGetPosition(pb->body);
    *position = (Vec3){ pos[0], pos[1], pos[2] };
}

void physics_get_rotation(PhysicsBody* pb, Vec3* rotation) {
    if (!pb || !pb->body || !rotation) return;

    const dReal* q = dBodyGetQuaternion(pb->body);

    double sinr_cosp = 2 * (q[0] * q[1] + q[2] * q[3]);
    double cosr_cosp = 1 - 2 * (q[1] * q[1] + q[2] * q[2]);
    rotation->x = atan2(sinr_cosp, cosr_cosp) * (180.0 / M_PI);

    double sinp = 2 * (q[0] * q[2] - q[3] * q[1]);
    if (fabs(sinp) >= 1)
        rotation->y = copysign(90.0, sinp) * (180.0 / M_PI);
    else
        rotation->y = asin(sinp) * (180.0 / M_PI);

    double siny_cosp = 2 * (q[0] * q[3] + q[1] * q[2]);
    double cosy_cosp = 1 - 2 * (q[2] * q[2] + q[3] * q[3]);
    rotation->z = atan2(siny_cosp, cosy_cosp) * (180.0 / M_PI);
}

void physics_get_linear_velocity(PhysicsBody* pb, Vec3* velocity) {
    if (!pb || !pb->body) return;
    const dReal* vel = dBodyGetLinearVel(pb->body);
    *velocity = (Vec3){ vel[0], vel[1], vel[2] };
}

void physics_set_position(PhysicsBody* pb, Vec3 position) {
    if (!pb || !pb->body) return;

    dBodySetPosition(pb->body, position.x, position.y, position.z);
    physics_wake_up(pb);
}

void physics_set_rotation(PhysicsBody* pb, Vec3 rotation) {
    if (!pb || !pb->body) return;

    double xr = degree_to_radian(rotation.x);
    double yr = degree_to_radian(rotation.y);
    double zr = degree_to_radian(rotation.z);

    dMatrix3 R;
    dRFromEulerAngles(R, xr, yr, zr);

    dBodySetRotation(pb->body, R);

    physics_wake_up(pb);
}

void physics_set_linear_velocity(PhysicsBody* pb, Vec3 velocity) {
    if (!pb || !pb->body) return;
    dBodySetLinearVel(pb->body, velocity.x, velocity.y, velocity.z);
    physics_wake_up(pb);
}

void physics_set_angular_velocity(PhysicsBody* pb, Vec3 velocity) {
    if (!pb || !pb->body) return;
    dBodySetAngularVel(pb->body, velocity.x, velocity.y, velocity.z);
    physics_wake_up(pb);
}

void physics_apply_force(PhysicsBody* pb, Vec3 force) {
    if (!pb || !pb->body) return;
    dBodyAddForce(pb->body, force.x, force.y, force.z);
    physics_wake_up(pb);
}

void physics_apply_torque(PhysicsBody* pb, Vec3 torque) {
    if (!pb || !pb->body) return;
    dBodyAddTorque(pb->body, torque.x, torque.y, torque.z);
    physics_wake_up(pb);
}

void physics_wake_up(PhysicsBody* pb) {
    if (!pb || !pb->body) return;
    dBodyEnable(pb->body);
    pb->is_sleeping = false;
}

void physics_disable_rotation(PhysicsBody* pb) {
    if (!pb || !pb->body) return;
    dBodySetFiniteRotationMode(pb->body, 1);
    dBodySetFiniteRotationAxis(pb->body, 0, 0, 1);
    dBodySetAngularVel(pb->body, 0, 0, 0);

    dJointID j = dJointCreateAMotor(dBodyGetWorld(pb->body), 0);
    dJointAttach(j, pb->body, 0);
    dJointSetAMotorMode(j, dAMotorEuler);
    dJointSetAMotorNumAxes(j, 3);
    dJointSetAMotorAxis(j, 0, 1, 1, 0, 0);
    dJointSetAMotorAxis(j, 1, 1, 0, 1, 0);
    dJointSetAMotorAxis(j, 2, 1, 0, 0, 1);
    for (int axis = 0; axis < 3; ++axis) {
        dJointSetAMotorParam(j, dParamLoStop + 2 * axis, 0);
        dJointSetAMotorParam(j, dParamHiStop + 2 * axis, 0);
    }
}