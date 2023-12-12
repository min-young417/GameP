using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;
using UnityEngine.UI;

public class P1_PlayerController : MonoBehaviour
{
    Animator animator;
    new Rigidbody2D rigidbody2D;
    private GameManager gameManager;
    private P1_PlayerController move;
    private int jumpCount = 0;

    void Start(){
        animator = GetComponent<Animator>();
        rigidbody2D = GetComponent<Rigidbody2D>();
        gameManager = FindObjectOfType<GameManager>();
        move = GetComponent<P1_PlayerController>();
    }

    void Update(){
        animator.SetBool("walk", false);
        if(Input.GetKey(KeyCode.RightArrow)){
            transform.Translate(Vector2.right * 0.03f);
            transform.localScale = new Vector3(1, 1, 1);
            animator.SetBool("walk", true);
        }

        if(Input.GetKey(KeyCode.LeftArrow)){
            transform.Translate(Vector2.left * 0.03f);
            transform.localScale = new Vector3(-1, 1, 1);
            animator.SetBool("walk", true);
        }

        if(Input.GetKeyDown(KeyCode.Space)){
            if(jumpCount < 2){
                jumpCount++;
                rigidbody2D.AddForce(Vector2.up * 400);
                rigidbody2D.gravityScale = 2;
                animator.SetTrigger("jump");
            }
        }

        if(Input.GetKey(KeyCode.Q)){
            rigidbody2D.AddForce(Vector2.up * 10);
            rigidbody2D.gravityScale = 2;
            animator.SetTrigger("jump");
        }
    }

    private void OnCollisionEnter2D(Collision2D collision){
        string objectName = collision.gameObject.name;
        
        if (objectName.StartsWith("coinBronze"))
        {
            GameObject.Destroy(collision.gameObject);
            gameManager.AddScore(1);
        }
        else if (objectName.StartsWith("coinSilver"))
        {
            GameObject.Destroy(collision.gameObject);
            gameManager.AddScore(2);
        }
        else if (objectName.StartsWith("coinGold"))
        {
            GameObject.Destroy(collision.gameObject);
            gameManager.AddScore(3);
        }
        else if (objectName.StartsWith("gemBlue"))
        {
            GameObject.Destroy(collision.gameObject);
            gameManager.AddScore(15);
        }
        else if (objectName.StartsWith("gemGreen"))
        {
            GameObject.Destroy(collision.gameObject);
            gameManager.AddScore(10);
        }
        else if (objectName.StartsWith("gemRed"))
        {
            GameObject.Destroy(collision.gameObject);
            gameManager.AddScore(5);
        }
        else if (objectName.StartsWith("slime"))
        {
            gameManager.OnPlayerDead();
            move.enabled = false;
            animator.enabled = false;
        }
        else if (objectName == "P3")
        {
            Debug.Log("클리어");
            SceneManager.LoadScene("Outro");
        }
        else if (objectName.StartsWith("Tilemap")){
            jumpCount = 0;
        }
    }
}
