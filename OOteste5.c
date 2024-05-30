#include <stdlib.h>
#include <string.h>
#include <cairo.h>
#include <gtk/gtk.h>
#include <math.h>

//Versão 5.
gchar  *winTitle    = "OO Teste(v5)" ;
glong   win_xlen    = 850+75;
glong   win_ylen    = 500 ;
gdouble pos1x       = 100+50.;
gdouble pos1y       = 50+175-2;

//Velocidade expressa em pixeis por segundo.
gdouble v           = 0   ;
gdouble temp        = 0   ;
gdouble T           = 0.5 ;
gdouble parteint    = 0   ;
gdouble partefrac   = 0   ;
gint    i           = 0   ;
gint    r[200]            ;
gdouble Vonda       = 200 ;
gdouble fmax1       = 0   ;
gdouble fmin1       = 0   ;
gint    start       = 0   ;

gdouble VFONTEini   = 0   ;
gdouble VONDAini    = 200 ;
gdouble FREQini     = 1/(0.5);

//Struct
typedef struct {
  //A utilizar.
  GtkWidget *VFONTEBUTTON;
  GtkWidget *VONDABUTTON;
  GtkWidget *FREQBUTTON;
  GtkWidget *EXEMPLO1BUTTON;
  GtkWidget *EXEMPLO2BUTTON;
  GtkWidget *EXEMPLO3BUTTON;
  GtkWidget *STARTBUTTON;
  GtkWidget *PAUSEBUTTON;
  GtkWidget *RESETBUTTON;
  //A utilizar.
} Boo;

//Função associada ao botão 'Começar'.
gboolean
buttonSTARTclick (GtkWidget *b, Boo *bu)
{
  
  gtk_widget_set_sensitive (b, FALSE);
  gtk_widget_set_sensitive (bu->VFONTEBUTTON, FALSE);
  gtk_widget_set_sensitive (bu->VONDABUTTON, FALSE);
  gtk_widget_set_sensitive (bu->FREQBUTTON, FALSE);
  gtk_widget_set_sensitive (bu->EXEMPLO1BUTTON, FALSE);
  gtk_widget_set_sensitive (bu->EXEMPLO2BUTTON, FALSE);
  gtk_widget_set_sensitive (bu->EXEMPLO3BUTTON, FALSE);
  gtk_widget_set_sensitive (bu->PAUSEBUTTON, TRUE);
  
  start=1;

  return FALSE;
}

//Função associada ao botão 'Pausar'.
gboolean
buttonPAUSEclick (GtkWidget *b, Boo *bu)
{
  gtk_widget_set_sensitive (b, FALSE);
  gtk_widget_set_sensitive (bu->STARTBUTTON, TRUE);
  
  start=0;
  
  return FALSE;
}

//Função associada ao botão 'Reset'.
gboolean
buttonRESETclick (GtkWidget *b, Boo *bu)
{
  start=0;
  
  pos1x =100+50.;

  /*Estas linhas permitem alterar os valores dos parâmetros para os 'valores iniciais', quando o botão 'Reset' é premido. É meramente uma opção ter isto desativado.
    v= VFONTEini;
    Vonda= VONDAini;
    T=(1/FREQini);

    gtk_spin_button_set_value ((GtkSpinButton*)bu->VFONTEBUTTON, VFONTEini);
    gtk_spin_button_set_value ((GtkSpinButton*)bu->VONDABUTTON, VONDAini);
    gtk_spin_button_set_value ((GtkSpinButton*)bu->FREQBUTTON, FREQini);*/

  gtk_widget_set_sensitive (bu->VFONTEBUTTON, TRUE);
  gtk_widget_set_sensitive (bu->VONDABUTTON, TRUE);
  gtk_widget_set_sensitive (bu->FREQBUTTON, TRUE);
  gtk_widget_set_sensitive (bu->EXEMPLO1BUTTON, TRUE);
  gtk_widget_set_sensitive (bu->EXEMPLO2BUTTON, TRUE);
  gtk_widget_set_sensitive (bu->EXEMPLO3BUTTON, TRUE);
  gtk_widget_set_sensitive (bu->STARTBUTTON, TRUE);
  gtk_widget_set_sensitive (bu->PAUSEBUTTON, FALSE);

  for(i=0;i<=199;i++)
    r[i]=0;

  temp=0;
    
  return FALSE;
}

//Função associada ao botão 'Exemplo 1'.
gboolean
buttonEXEMPLO1click (GtkWidget *b, Boo *bu)
{
  if (v==125 && Vonda==200 && T==0.10)
    return FALSE;
  else
    {
      v= 125;
      Vonda= 200;
      T=(1/10);

      gtk_spin_button_set_value ((GtkSpinButton*)bu->VFONTEBUTTON, 125);
      gtk_spin_button_set_value ((GtkSpinButton*)bu->VONDABUTTON, 200);
      gtk_spin_button_set_value ((GtkSpinButton*)bu->FREQBUTTON, 10);
  
      return FALSE;
    }
}

//Função associada ao botão 'Exemplo 2'.
gboolean
buttonEXEMPLO2click (GtkWidget *b, Boo *bu)
{
  if (v==350 && Vonda==200 && T==((gdouble)1/15))
    return FALSE;
  else
    {
      v= 350;
      Vonda= 200;
      T=(1/15);

      gtk_spin_button_set_value ((GtkSpinButton*)bu->VFONTEBUTTON, 350);
      gtk_spin_button_set_value ((GtkSpinButton*)bu->VONDABUTTON, 200);
      gtk_spin_button_set_value ((GtkSpinButton*)bu->FREQBUTTON, 15);
  
      return FALSE;
    }
}

//Função associada ao botão 'Exemplo 3'.
gboolean
buttonEXEMPLO3click (GtkWidget *b, Boo *bu)
{
  if (v==200 && Vonda==200 && T==((gdouble)1/20))
    return FALSE;
  else
    {
      v= 200;
      Vonda= 200;
      T=(1/20);

      gtk_spin_button_set_value ((GtkSpinButton*)bu->VFONTEBUTTON, 200);
      gtk_spin_button_set_value ((GtkSpinButton*)bu->VONDABUTTON, 200);
      gtk_spin_button_set_value ((GtkSpinButton*)bu->FREQBUTTON, 20);

      return FALSE;
    }
}


gboolean
buttonsCH (GtkWidget *b, Boo *bu)
{
  v=gtk_spin_button_get_value((GtkSpinButton*)bu->VFONTEBUTTON);
  Vonda=gtk_spin_button_get_value((GtkSpinButton*)bu->VONDABUTTON);
  T= (1/(gtk_spin_button_get_value((GtkSpinButton*)bu->FREQBUTTON)));
  
  return FALSE;
}


gboolean 
cb_draw_event (GtkWidget  *darea , 
               cairo_t    *cr    , 
               gpointer    data  )
{      
  gchar          texto[128] ;
  gchar          texto2[128] ;
  //static double  pos1x = 100., pos1y = 50+175.;
  GtkAllocation  alloc1 ;

  if(start==1){
    temp+= 0.01;}
  
  gtk_widget_get_allocation (darea, &alloc1);

  fmax1= Vonda/(Vonda-v);
  fmin1= Vonda/(Vonda+v);

  //Para testes, caso necessário.
  /*printf("\nfmax1: %.2f , fmin1: %.2f\n\n", fmax1, fmin1);
    printf("\n1: %lf  e  2: %lf", 0.5/fmax1, 0.5*fmax1);
    printf("\na: %lf  e  b: %lf\n\n", 0.5/fmin1, 0.5*fmin1);*/

  cairo_select_font_face (cr, "Courier", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
  cairo_set_font_size (cr, 15);
  cairo_set_source_rgb (cr, 0., 0., 1.);
  cairo_move_to (cr, 51+25., 22.);
  sprintf (texto2, "Tempo: %.1lf s    Freq. máx.: %.2lf Hz    Freq. Mín.: %.2lf Hz", temp, fmax1*(1/T), fmin1*(1/T));
  cairo_show_text (cr, texto2);

  //Cálculo e impressão do ângulo de 'Mach'.
  if(v>Vonda)
    {
      cairo_set_source_rgb (cr, 0., 0.8, 0.);
      cairo_move_to (cr, alloc1.width-239, alloc1.height-50);
      sprintf (texto2, "Ângulo de 'Mach': %.2lfº", asin(Vonda/v)*(180/M_PI));
      cairo_show_text (cr, texto2);
    }
      
  //***
  //Grelha do background
  
  //Parte horizontal
  cairo_set_source_rgb (cr, 0, 0., 0.);
  cairo_set_line_width (cr, 0.4);
  cairo_move_to(cr, 0., alloc1.height-450);
  cairo_rel_line_to(cr, 750., 0.);
  cairo_stroke(cr);
  cairo_set_font_size (cr, 11);
  cairo_move_to (cr, 5., alloc1.height-450+3.);
  strcpy (texto, "200");
  cairo_show_text (cr, texto);
  cairo_stroke (cr);
  
  cairo_move_to(cr, 0., alloc1.height-400);
  cairo_rel_line_to(cr, 750., 0.);
  cairo_stroke(cr);
  cairo_move_to (cr, 5., alloc1.height-400+3.);
  strcpy (texto, "150");
  cairo_show_text (cr, texto);
  cairo_stroke (cr);

  cairo_move_to(cr, 0., alloc1.height-350);
  cairo_rel_line_to(cr, 750., 0.);
  cairo_stroke(cr);
  cairo_move_to (cr, 5., alloc1.height-350+3.);
  strcpy (texto, "100");
  cairo_show_text (cr, texto);
  cairo_stroke (cr);

  cairo_move_to(cr, 0., alloc1.height-300);
  cairo_rel_line_to(cr, 750., 0.);
  cairo_stroke(cr);
  cairo_move_to (cr, 5., alloc1.height-300+3.);
  strcpy (texto, "50");
  cairo_show_text (cr, texto);
  cairo_stroke (cr);

  cairo_move_to(cr, 0., alloc1.height-250);
  cairo_rel_line_to(cr, 750., 0.);
  cairo_stroke(cr);
  cairo_move_to (cr, 5., alloc1.height-250+3.);
  strcpy (texto, "0");
  cairo_show_text (cr, texto);
  cairo_stroke (cr);

  cairo_move_to(cr, 0., alloc1.height-200);
  cairo_rel_line_to(cr, 750., 0.);
  cairo_stroke(cr);
  cairo_move_to (cr, 5., alloc1.height-200+3.);
  strcpy (texto, "-50");
  cairo_show_text (cr, texto);
  cairo_stroke (cr);

  cairo_move_to(cr, 0., alloc1.height-150);
  cairo_rel_line_to(cr, 750., 0.);
  cairo_stroke(cr);
  cairo_move_to (cr, 5., alloc1.height-150+3.);
  strcpy (texto, "-100");
  cairo_show_text (cr, texto);
  cairo_stroke (cr);

  cairo_move_to(cr, 0., alloc1.height-100);
  cairo_rel_line_to(cr, 750., 0.);
  cairo_stroke(cr);
  cairo_move_to (cr, 5., alloc1.height-100+3.);
  strcpy (texto, "-150");
  cairo_show_text (cr, texto);
  cairo_stroke (cr);

  cairo_move_to(cr, 0., alloc1.height-50);
  cairo_rel_line_to(cr, 750., 0.);
  cairo_stroke(cr);
  cairo_move_to (cr, 5., alloc1.height-50+3.);
  strcpy (texto, "-200");
  cairo_show_text (cr, texto);
  cairo_stroke (cr);

  //Parte Vertical
  cairo_set_source_rgb (cr, 0, 0., 0.);
  cairo_set_line_width (cr, 0.4);
  cairo_move_to(cr, 50., 0.);
  cairo_rel_line_to(cr, 0., 550.);
  cairo_stroke(cr);
  cairo_set_font_size (cr, 11);
  cairo_move_to (cr, 50-17., alloc1.height - 5.);
  strcpy (texto, "-100");
  cairo_show_text (cr, texto);
  cairo_stroke (cr);
  
  cairo_move_to(cr, 100., 0.);
  cairo_rel_line_to(cr, 0., 550.);
  cairo_stroke(cr);
  cairo_move_to (cr, 100-12., alloc1.height - 5.);
  strcpy (texto, "-50");
  cairo_show_text (cr, texto);
  cairo_stroke (cr);

  cairo_move_to(cr, 150., 0.);
  cairo_rel_line_to(cr, 0., 550.);
  cairo_stroke(cr);
  cairo_move_to (cr, 150-3., alloc1.height - 5.);
  strcpy (texto, "0");
  cairo_show_text (cr, texto);
  cairo_stroke (cr);

  cairo_move_to(cr, 200., 0);
  cairo_rel_line_to(cr, 0., 550.);
  cairo_stroke(cr);
  cairo_move_to (cr, 200-6., alloc1.height - 5.);
  strcpy (texto, "50");
  cairo_show_text (cr, texto);
  cairo_stroke (cr);

  cairo_move_to(cr, 250., 0);
  cairo_rel_line_to(cr, 0., 550.);
  cairo_stroke(cr);
  cairo_move_to (cr, 250-11., alloc1.height - 5.);
  strcpy (texto, "100");
  cairo_show_text (cr, texto);
  cairo_stroke (cr);

  cairo_move_to(cr, 300., 0);
  cairo_rel_line_to(cr, 0., 550.);
  cairo_stroke(cr);
  cairo_move_to (cr, 300-11., alloc1.height - 5.);
  strcpy (texto, "150");
  cairo_show_text (cr, texto);
  cairo_stroke (cr);

  cairo_move_to(cr, 350., 0);
  cairo_rel_line_to(cr, 0., 550.);
  cairo_stroke(cr);
  cairo_move_to (cr, 350-11., alloc1.height - 5.);
  strcpy (texto, "200");
  cairo_show_text (cr, texto);
  cairo_stroke (cr);

  cairo_move_to(cr, 400., 0);
  cairo_rel_line_to(cr, 0., 550.);
  cairo_stroke(cr);
  cairo_move_to (cr, 400-11., alloc1.height - 5.);
  strcpy (texto, "250");
  cairo_show_text (cr, texto);
  cairo_stroke (cr);

  cairo_move_to(cr, 450., 0);
  cairo_rel_line_to(cr, 0., 550.);
  cairo_stroke(cr);
  cairo_move_to (cr, 450-11., alloc1.height - 5.);
  strcpy (texto, "300");
  cairo_show_text (cr, texto);
  cairo_stroke (cr);

  cairo_move_to(cr, 500., 0);
  cairo_rel_line_to(cr, 0., 550.);
  cairo_stroke(cr);
  cairo_move_to (cr, 500-11., alloc1.height - 5.);
  strcpy (texto, "350");
  cairo_show_text (cr, texto);
  cairo_stroke (cr);

  cairo_move_to(cr, 550., 0);
  cairo_rel_line_to(cr, 0., 550.);
  cairo_stroke(cr);
  cairo_move_to (cr, 550-11., alloc1.height - 5.);
  strcpy (texto, "400");
  cairo_show_text (cr, texto);
  cairo_stroke (cr);

  cairo_move_to(cr, 600., 0);
  cairo_rel_line_to(cr, 0., 550.);
  cairo_stroke(cr);
  cairo_move_to (cr, 600-11., alloc1.height - 5.);
  strcpy (texto, "450");
  cairo_show_text (cr, texto);
  cairo_stroke (cr);

  cairo_move_to(cr, 650., 0);
  cairo_rel_line_to(cr, 0., 550.);
  cairo_stroke(cr);
  cairo_move_to (cr, 650-11., alloc1.height - 5.);
  strcpy (texto, "500");
  cairo_show_text (cr, texto);
  cairo_stroke (cr);
  //***

  //Fonte emissora.
  cairo_set_source_rgb (cr, 1, 0.2, 0.9);
  cairo_set_line_width (cr, 2.0);
  cairo_arc (cr, pos1x, pos1y, 5, 0, 2 * M_PI);
  cairo_fill (cr);
  cairo_stroke (cr);
  
  // Actualizacao da posicao da fonte emissora.
  if(start==1){
    pos1x +=0.01*v;}


  // Wavelets de Huygens.
  if(start==1)
    {
      partefrac = modf((gdouble)(temp/T), &parteint);
      if(partefrac<2e-1)
	{
	  for(i=0;i<=199;i++)
	    {
	      //Para testes, caso necessário.
	      /*printf("parteint= %.1lf i= %d\n", parteint, i);*/
	      if(parteint==i)
		{
		  r[i]=1;
		  //Para testes, caso necessário.
		  //printf("\n");
		  /* printf("r[%d]=%d\n", i, r[i]);
		     printf("***\n\n");*/
		  break;
		}
	    }
	}
    }

  for(i=0;i<=199;i++)
    {
      if(r[i]==1)
	{
	  //Círculo principal.
	  //Parte de baixo.
	  cairo_set_source_rgb (cr, 0.5, 0., 0.5);
	  cairo_set_line_width (cr, 2.0);
	  cairo_arc (cr, pos1x-((temp-T)*v)+i*T*v, pos1y, (temp-T)*Vonda-i*T*Vonda, 0.05, M_PI-0.05);
	  cairo_stroke (cr);
	  //Parte de cima.
	  cairo_arc (cr, pos1x-((temp-T)*v)+i*T*v, pos1y, (temp-T)*Vonda-i*T*Vonda, M_PI+0.05, 2* M_PI-0.05);
	  cairo_stroke (cr);


	  //Parte do arco modificada para ilustrar o efeito de Doppler (variação da frequência).
	  //Parte da direita.
	  if(v <= Vonda) cairo_set_source_rgb (cr, 0.5/fmax1, 0, 0.5*fmax1);//Alteração da cor para refletir varição da frequência (red/blueshift).
	  else cairo_set_source_rgb (cr, -0.5/fmax1, 0, -0.5*fmax1);//Alteração da cor para refletir varição da frequência (red/blueshift) (caso em que vfonte > vonda).
	  
	  cairo_set_line_width (cr, 3.5);
	  cairo_arc (cr, pos1x-((temp-T)*v)+i*T*v, pos1y, (temp-T)*Vonda-i*T*Vonda, -0.05, 0.05);
	  cairo_stroke (cr);
	  
	  //Parte da esquerda.
	  cairo_set_source_rgb (cr, 0.5/fmin1, 0, 0.5*fmin1);//Alteração da cor para refletir varição da frequência (red/blueshift).
	  cairo_arc (cr, pos1x-((temp-T)*v)+i*T*v, pos1y, (temp-T)*Vonda-i*T*Vonda, M_PI-0.05, M_PI+0.05);
	  cairo_stroke (cr);
	  
	}
    }
    
  //Representação do ângulo de 'Mach'.
  if(v>Vonda)
    {
      if(r[1]==1)
	{
	  cairo_move_to (cr, 150+T*v+(temp-T)*Vonda, pos1y);
	  cairo_set_source_rgb (cr, 0., 0.8, 0.);
	  cairo_set_line_width (cr, 4.5);
	  cairo_arc (cr, pos1x, pos1y, pos1x-(150+T*v+(temp-T)*Vonda), M_PI, M_PI+asin(Vonda/v));
	  cairo_stroke (cr);
	}
    }
  
  return(FALSE);
}



gboolean 
time_handler (GtkWidget *widget)
{
  if ((!GTK_IS_WIDGET(widget)) || (!gtk_widget_get_window (widget)))
    return FALSE;
  
  gtk_widget_queue_draw(widget);

  return TRUE;
}



int main(int argc, char *argv[])
{
  GtkWidget *window, *darea, *HBOX, *FRAME_SIM, *FRAME_PAR, *VBOX, *LABEL_VFONTE, *LABEL_VONDA, *LABEL_FREQ, *SEP, *SEP2;
  Boo buttons;

  //Para testes, caso necessário.
  /* printf("\n");*/
   
  for(i=0;i<=199;i++)
    {
      r[i]=0;
      //Para testes, caso necessário.
      /*printf("r[%d]=%d\n", i, r[i]);*/
    }

  //Para testes, caso necessário.
  /* printf("\n");*/
  
  gtk_init(&argc, &argv);

  window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_window_set_position (GTK_WINDOW(window), GTK_WIN_POS_CENTER);
  gtk_window_set_default_size (GTK_WINDOW(window), win_xlen, win_ylen);
  gtk_window_set_title (GTK_WINDOW(window), winTitle);
  gtk_window_set_resizable((GtkWindow*)window,FALSE);

  //Box maior.
  HBOX = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 10);
  gtk_container_set_border_width (GTK_CONTAINER (HBOX), 10);
  gtk_widget_set_size_request (HBOX, 1000, 500);
  gtk_container_add (GTK_CONTAINER (window), HBOX);

  //Frame simulação.
  FRAME_SIM = gtk_frame_new ("Simulação");
  gtk_box_pack_start (GTK_BOX (HBOX), FRAME_SIM, TRUE, TRUE, 0);

  darea = gtk_drawing_area_new ();
  gtk_container_add (GTK_CONTAINER (FRAME_SIM), darea);

  //Frame parâmetros.
  FRAME_PAR = gtk_frame_new ("Parâmetros");
  gtk_box_pack_start (GTK_BOX (HBOX), FRAME_PAR, FALSE, TRUE, 0);

  //Box vertical com os parâmetros.
  VBOX = gtk_box_new (GTK_ORIENTATION_VERTICAL, 10);
  gtk_container_set_border_width (GTK_CONTAINER (VBOX), 10);
  gtk_widget_set_size_request (HBOX, 250, 500);
  gtk_container_add (GTK_CONTAINER (FRAME_PAR), VBOX);

  //Parâmetros.
  //Velocidade da fonte emissora.
  LABEL_VFONTE= gtk_label_new ("Velocidade da fonte: (px/s)");
  gtk_box_pack_start (GTK_BOX (VBOX), LABEL_VFONTE, FALSE, TRUE, 0);
  buttons.VFONTEBUTTON=gtk_spin_button_new_with_range (0, 500, 25);
  gtk_spin_button_set_value (GTK_SPIN_BUTTON (buttons.VFONTEBUTTON), VFONTEini);
  gtk_box_pack_start (GTK_BOX (VBOX), buttons.VFONTEBUTTON, FALSE, TRUE, 0);
  g_signal_connect(G_OBJECT(buttons.VFONTEBUTTON), "value-changed", G_CALLBACK(buttonsCH), &buttons);

  //Velocidade de propagação da onda no meio.
  LABEL_VONDA= gtk_label_new ("Velocidade da onda: (px/s)");
  gtk_box_pack_start (GTK_BOX (VBOX), LABEL_VONDA, FALSE, TRUE, 0);
  buttons.VONDABUTTON=gtk_spin_button_new_with_range (0, 500, 25);
  gtk_spin_button_set_value (GTK_SPIN_BUTTON (buttons.VONDABUTTON), VONDAini);
  gtk_box_pack_start (GTK_BOX (VBOX), buttons.VONDABUTTON, FALSE, TRUE, 0);
  g_signal_connect(G_OBJECT(buttons.VONDABUTTON), "value-changed", G_CALLBACK(buttonsCH), &buttons);

  //Frequência da fonte emissora.
  LABEL_FREQ= gtk_label_new ("Frequência da emissão: (Hz)");
  gtk_box_pack_start (GTK_BOX (VBOX), LABEL_FREQ, FALSE, TRUE, 0);
  buttons.FREQBUTTON=gtk_spin_button_new_with_range (0, 20, 1);
  gtk_spin_button_set_value (GTK_SPIN_BUTTON (buttons.FREQBUTTON), FREQini);
  gtk_box_pack_start (GTK_BOX (VBOX), buttons.FREQBUTTON, FALSE, TRUE, 0);
  g_signal_connect(G_OBJECT(buttons.FREQBUTTON), "value-changed", G_CALLBACK(buttonsCH), &buttons);

  //Separador 1.
  SEP = gtk_separator_new (GTK_ORIENTATION_HORIZONTAL);
  gtk_box_pack_start(GTK_BOX(VBOX), SEP, FALSE, TRUE, 0);
  
  //Butões com exemplos.
  //Exemplo 1.
  buttons.EXEMPLO1BUTTON= gtk_button_new_with_label("Exemplo 1");
  gtk_box_pack_start(GTK_BOX(VBOX), buttons.EXEMPLO1BUTTON, FALSE, TRUE, 0);
  g_signal_connect(G_OBJECT(buttons.EXEMPLO1BUTTON), "clicked", G_CALLBACK(buttonEXEMPLO1click),&buttons);

  //Exemplo 2.
  buttons.EXEMPLO2BUTTON= gtk_button_new_with_label("Exemplo 2");
  gtk_box_pack_start(GTK_BOX(VBOX), buttons.EXEMPLO2BUTTON, FALSE, TRUE, 0);
  g_signal_connect(G_OBJECT(buttons.EXEMPLO2BUTTON), "clicked", G_CALLBACK(buttonEXEMPLO2click),&buttons);

  //Exemplo 3.
  buttons.EXEMPLO3BUTTON= gtk_button_new_with_label("Exemplo 3");
  gtk_box_pack_start(GTK_BOX(VBOX), buttons.EXEMPLO3BUTTON, FALSE, TRUE, 0);
  g_signal_connect(G_OBJECT(buttons.EXEMPLO3BUTTON), "clicked", G_CALLBACK(buttonEXEMPLO3click),&buttons);

  //Separador 2.
  SEP2 = gtk_separator_new (GTK_ORIENTATION_HORIZONTAL);
  gtk_box_pack_start(GTK_BOX(VBOX), SEP2, FALSE, TRUE, 0);

  //Butões para o controlo da simulação.
  //Começar/recomeçar.
  buttons.STARTBUTTON= gtk_button_new_with_label("Começar");
  gtk_box_pack_start(GTK_BOX(VBOX), buttons.STARTBUTTON, FALSE, TRUE, 0);
  g_signal_connect(G_OBJECT(buttons.STARTBUTTON), "clicked", G_CALLBACK(buttonSTARTclick),&buttons);

  //Parar/pausar.
  buttons.PAUSEBUTTON= gtk_button_new_with_label("Pausar");
  gtk_box_pack_start(GTK_BOX(VBOX), buttons.PAUSEBUTTON, FALSE, TRUE, 0);
  gtk_widget_set_sensitive (buttons.PAUSEBUTTON, FALSE);
  g_signal_connect(G_OBJECT(buttons.PAUSEBUTTON), "clicked", G_CALLBACK(buttonPAUSEclick),&buttons);

  //Reset.
  buttons.RESETBUTTON= gtk_button_new_with_label("Reset");
  gtk_box_pack_start(GTK_BOX(VBOX), buttons.RESETBUTTON, FALSE, TRUE, 0);
  g_signal_connect(G_OBJECT(buttons.RESETBUTTON), "clicked", G_CALLBACK(buttonRESETclick),&buttons);
  
  g_signal_connect (G_OBJECT(darea), "draw", G_CALLBACK(cb_draw_event), NULL);  
  g_signal_connect (G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);

  g_timeout_add (10, (GSourceFunc) time_handler, darea);
 
  gtk_widget_show_all(window);
  gtk_main();

  return 0;
}
