/**
 * @file <argos3/plugins/simulator/physics_engines/dynamics3d/dynamics3d_cylinder_model.cpp>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#include "dynamics3d_cylinder_model.h"

#include <argos3/plugins/simulator/physics_engines/dynamics3d/dynamics3d_engine.h>

namespace argos {

   /****************************************/
   /****************************************/

   CDynamics3DCylinderModel::CDynamics3DCylinderModel(CDynamics3DEngine& c_engine,
                                                      CCylinderEntity& c_cylinder) :
      CDynamics3DModel(c_engine, c_cylinder.GetEmbodiedEntity(), c_cylinder.GetId()),
      m_cCylinderEntity(c_cylinder) {
      /* Get origin anchor and register its update method */
      const SAnchor& sOrigin = GetEmbodiedEntity().GetOriginAnchor();
      RegisterAnchorMethod<CDynamics3DCylinderModel>(sOrigin,
                                                     &CDynamics3DCylinderModel::UpdateOriginAnchor);
      /* When defining size of objects we must manually swap the Z and Y components */
      m_pcCollisionShape = m_cEngine.GetCylinderShapeManager().RequestShape(
         btVector3(c_cylinder.GetRadius(),
                   c_cylinder.GetHeight() * 0.5f,
                   c_cylinder.GetRadius()));
      btTransform cCylinderGeometricOffset(
         btQuaternion(0.0f, 0.0f, 0.0f, 1.0f), 
         btVector3(0.0f, -c_cylinder.GetHeight() * 0.5f, 0.0f));
      Real fMass = c_cylinder.GetEmbodiedEntity().IsMovable() ? c_cylinder.GetMass() : 0.0f;
      m_vecLocalBodies.push_back(new CDynamics3DBody(this,
                                                     "cylinder",
                                                     m_pcCollisionShape,
                                                     btTransform::getIdentity(),
                                                     cCylinderGeometricOffset,
                                                     fMass));
      /* move the model to the specified coordinates */
      SetModelCoordinates(btTransform(ARGoSToBullet(GetEmbodiedEntity().GetOriginAnchor().Orientation),
                                      ARGoSToBullet(GetEmbodiedEntity().GetOriginAnchor().Position)));
   }
   
   /****************************************/
   /****************************************/
   
   CDynamics3DCylinderModel::~CDynamics3DCylinderModel() {
      m_cEngine.GetCylinderShapeManager().ReleaseShape(m_pcCollisionShape);
    }
   
   /****************************************/
   /****************************************/

   void CDynamics3DCylinderModel::UpdateEntityStatus() {
      if(m_cCylinderEntity.GetEmbodiedEntity().IsMovable()) {      
         CPhysicsModel::UpdateEntityStatus();
     }
   }

   /****************************************/
   /****************************************/

   btTransform CDynamics3DCylinderModel::GetModelCoordinates() const {
      return m_vecLocalBodies[0]->GetMotionStateTransform();
   }

   /****************************************/
   /****************************************/

   void CDynamics3DCylinderModel::UpdateOriginAnchor(SAnchor& s_anchor) {
      s_anchor.Position = BulletToARGoS(GetModelCoordinates().getOrigin());
      s_anchor.Orientation = BulletToARGoS(GetModelCoordinates().getRotation());
   }

   /****************************************/
   /****************************************/

   REGISTER_STANDARD_DYNAMICS3D_OPERATIONS_ON_ENTITY(CCylinderEntity, CDynamics3DCylinderModel);

}