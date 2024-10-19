using System;
using System.IO;
using System.Collections.Generic;
using System.Text;
using UnityEngine;

namespace AlmightyMatrixEditor.EngineComponents
{
    //AMW MATRIX
    [Serializable]
    public class GodRay_Matrix : MonoBehaviour
    {
        public float gravityAmount = -10;
        public int sceneID = 0;
        public Camera cam;
        public enum CameraType
        {
            Perspective = 0,
            Orthographic = 1
        }
        public CameraType cameraType;
        public List<GodRay_GameObject> objectsInScene = new List<GodRay_GameObject>();

        public byte[] ExportGodRayMatrix()
        {
                MemoryStream ms = new MemoryStream();
                BinaryWriter bw = new BinaryWriter(ms);
                bw.Write((ushort)0xA0A0);//start of matrix
                bw.Write(sceneID);//scene
                bw.Write((int)cameraType);//camera
                bw.Write(cam.transform.position.x);//position
                bw.Write(cam.transform.position.y);//flip y axis
                bw.Write(cam.transform.position.z);
                bw.Write(cam.transform.rotation.x);//rotation
                bw.Write(cam.transform.rotation.y);
                bw.Write(cam.transform.rotation.z);
                bw.Write(cam.transform.rotation.w);
                bw.Write(gravityAmount);//gravity
                bw.Write((ushort)0xA4A4);
                bw.Write(objectsInScene.Count);
                for (int i = 0; i < objectsInScene.Count; i++)
                {
                   bw.Write(objectsInScene[i].ExportGodRayGameObject());
                }
                bw.Close();
                return ms.ToArray();
        }
    }
    //BASE GAME OBJECT
    //GameObject Default : 0x01
    //Anything else needs custom implementaion
    [Serializable]
    public class GodRay_GameObject : MonoBehaviour //Add custom class ID
    {
        //steals data from gameobject
        int ClassID = 0;
        int objectID = 0;
        int parentID = -1;
        int modelID;
        public List<GodRay_Component> awmGameObjectComponents;
        public string awmObjectName = "GODRAY_BASE_GAMEOBJECT";
       //export
       //component ID, number of components, write component data
        public byte[] ExportGodRayGameObject()
        {
            MemoryStream ms = new MemoryStream();
            BinaryWriter bw = new BinaryWriter(ms);
            bw.Write((ushort)0xAAAA);//start of object
            bw.Write(ClassID);
            bw.Write(objectID);
            //position, rotation, scale
            bw.Write(transform.position.x);
            bw.Write(transform.position.y); 
            bw.Write(transform.position.z);
            bw.Write(transform.rotation.x);
            bw.Write(transform.rotation.y);
            bw.Write(transform.rotation.z);
            bw.Write(transform.rotation.w);
            bw.Write(transform.localScale.x);
            bw.Write(transform.localScale.y);
            bw.Write(transform.localScale.z);
            bw.Write(modelID);
            bw.Write(parentID);
            bw.Write(awmGameObjectComponents.Count);
            if(awmGameObjectComponents.Count > 0)
            {
                for(int j = 0; j < awmGameObjectComponents.Count; j++)//loop each component
                {
                    bw.Write(awmGameObjectComponents[j].ExportComponentData());
                }
                bw.Close();

            }
             return ms.ToArray();
        }
    }
    //COMPONENTS
    /*
    BPhysics : 0x01
    Animation : 0x02
    Audio : 0x03
    Particle : 0x04

    */
    [Serializable]
    public class GodRay_Component : ScriptableObject
    {
     
        public virtual byte[] ExportComponentData() 
        {
            return null;
        }

    }
    //BPhysics
    [Serializable]
    [CreateAssetMenu(fileName = "New GodRay Physics Component", menuName = "Almighty Matrix Editor/Physics Components")]
    public class GodRay_PhysicsComponent : GodRay_Component
    {
    
        public enum Shape
        {
            Box = 0,
            Sphere = 1
        }
        public float mass = 1.0f;
        public float radius = 1.0f;
        public Vector3 size;
        public bool isTrigger, isKinematic;
        public Shape shape;
       public override byte[] ExportComponentData() 
       {
        
            MemoryStream ms = new MemoryStream();
            BinaryWriter bw = new BinaryWriter(ms);
            bw.Write((ushort)0xCBFF);
            bw.Write(mass);
            bw.Write(size.x);
            bw.Write(size.y);
            bw.Write(size.z);
            bw.Write(isTrigger);
            bw.Write(isKinematic);
            bw.Write((int)shape);
            bw.Close();
            return ms.ToArray();
       }

    }
    //Animation
    [Serializable]
    [CreateAssetMenu(fileName = "New GodRay Animator Component", menuName = "Almighty Matrix Editor/Animation Components")]
    public class GodRay_AnimatorComponent : GodRay_Component
    {
    
        public AnimationFile animationToLoadID;
        public override byte[] ExportComponentData() 
       {
            
            MemoryStream ms = new MemoryStream();
            BinaryWriter bw = new BinaryWriter(ms);
            bw.Write((ushort)0xCAFF);
            bw.Write((int)animationToLoadID);
            bw.Close();
            return ms.ToArray();
       }
    }
    //Audio
    [Serializable]
    [CreateAssetMenu(fileName = "New AWM Audio Component", menuName = "Almighty Matrix Editor/Audio Components")]
    public class GodRay_AudioComponent : GodRay_Component
    {
   
        public AudioFile audioFileToLoadID;
         public override byte[] ExportComponentData() 
       {
  
            MemoryStream ms = new MemoryStream();
            BinaryWriter bw = new BinaryWriter(ms);
            bw.Write((ushort)0xCCFF);
            bw.Write((int)audioFileToLoadID);
            bw.Close();
            return ms.ToArray();
       }
    }
    //Particle TODO
}   


//Object Count
//First Object = 0xAAAA
//ComponentID
//Switch to Component to Handle that data
//ID = 0x01
//BPHYSICS
//Shape, Mass, 
//ID = 0x02
//ANIMATION
//ID = 0x03
//AUDIO
//ID = 0x04
//PARTICLEs