using System;
using System.IO;
using System.Collections.Generic;
using System.Text;
using UnityEngine;

namespace AlmightyMatrixEditor.EngineComponents
{
    //AMW MATRIX
    [Serializable]
    public class AWM_Matrix : MonoBehaviour
    {
        public int gravityAmount = -10;
        public Vector4 sceneColor;
        public string skyBoxName, meshName;

        public byte[] ExportAWMMatrix()
        {
                MemoryStream ms = new MemoryStream();
                BinaryWriter bw = new BinaryWriter(ms);
                bw.Write(0xA4A4);//start of matrix
                bw.Write(transform.position.x);//position
                bw.Write(transform.position.y);
                bw.Write(transform.position.z);
                bw.Write(transform.rotation.x);//rotation
                bw.Write(transform.rotation.y);
                bw.Write(transform.rotation.z);
                bw.Write(transform.rotation.w);
                bw.Write(transform.localScale.x);//scale
                bw.Write(transform.localScale.y);
                bw.Write(transform.localScale.z);
                bw.Write(gravityAmount);//gravity
                bw.Write(sceneColor.x);//Scene color data
                bw.Write(sceneColor.y);
                bw.Write(sceneColor.z);
                bw.Write(sceneColor.w);
                bw.Write(skyBoxName);//skybox name count, then chars
                bw.Write(meshName);//mesh name count, then chars
                bw.Close();
                return ms.ToArray();
        }
    }
    //BASE GAME OBJECT
    //GameObject Default : 0x01
    //Anything else needs custom implementaion
    [Serializable]
    public class AWM_GameObject : MonoBehaviour //Add custom class ID
    {
        //steals data from gameobject
        byte ClassID = 0x01;
        public List<AWM_Component> awmGameObjectComponents;
        public string awmObjectName = "AWM_BASE_GAMEOBJECT";
       //export
       //component ID, number of components, write component data
        public void ExportAWMGameObject()
        {
            MemoryStream ms = new MemoryStream();
            BinaryWriter bw = new BinaryWriter(ms);
            bw.Write(0xAAAA);//start of object
            bw.Write(ClassID);
            bw.Write(awmObjectName);//object name
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
            if(awmGameObjectComponents.Count > 0)
            {
            
                bw.Write(awmGameObjectComponents.Count);//number of components
                for(int i = 0; i < awmGameObjectComponents.Count; i++)
                {
              
                for(int j = 0; j < awmGameObjectComponents.Count; j++)//loop each component
                {
                    bw.Write(awmGameObjectComponents[j].ExportComponentData());
                }
                bw.Close();
                
                }
            }
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
    public class AWM_Component : ScriptableObject
    {
        byte componentID;
        public byte GetComponentID()
        {
            return componentID;
        }
        public void SetComponentID(byte componentID)
        {
            this.componentID = componentID;
        }
        public virtual byte[] ExportComponentData() 
        {
            return null;
        }

    }
    //BPhysics
    [Serializable]
    [CreateAssetMenu(fileName = "New AWM BPhysics Component", menuName = "Almighty Matrix Editor/Bullet Physics Components")]
    public class BulletPhysicsComponent : AWM_Component
    {
    
       public enum PhysicsShape
        {
            Shpere,
            Cube,
            Plane
        }
       public PhysicsShape shape;
       public float mass = 1.0f;
       public override byte[] ExportComponentData() 
       {
            SetComponentID(0x01);
            MemoryStream ms = new MemoryStream();
            BinaryWriter bw = new BinaryWriter(ms);
            bw.Write((byte)GetComponentID());
            bw.Write((byte)shape);
            bw.Write(mass);
            bw.Close();
            return ms.ToArray();
       }

    }
    //Animation
    [Serializable]
    [CreateAssetMenu(fileName = "New AWM Animator Component", menuName = "Almighty Matrix Editor/Animation Components")]
    public class AnimatorComponent : AWM_Component
    {
        public List<AnimationData> animationData;
        public override byte[] ExportComponentData() 
       {
            SetComponentID(0x02);
            MemoryStream ms = new MemoryStream();
            BinaryWriter bw = new BinaryWriter(ms);
            bw.Write((byte)GetComponentID());
            bw.Write(animationData.Count);
            for (int i = 0; i < animationData.Count; i++)
            {
                bw.Write(animationData[i].animationName);
                bw.Write(animationData[i].pathToFile);
            }
            bw.Close();
            return ms.ToArray();
       }
    }
    [Serializable]
    
    public struct AnimationData
    {
        public string animationName;
        public string pathToFile;//path to compiled animation object
    }
    //Audio
    [Serializable]
    [CreateAssetMenu(fileName = "New AWM Audio Component", menuName = "Almighty Matrix Editor/Audio Components")]
    public class AudioComponent : AWM_Component
    {
        public string fileName;
         public override byte[] ExportComponentData() 
       {
            SetComponentID(0x03);
            MemoryStream ms = new MemoryStream();
            BinaryWriter bw = new BinaryWriter(ms);
            bw.Write((byte)GetComponentID());
            bw.Write(fileName);
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