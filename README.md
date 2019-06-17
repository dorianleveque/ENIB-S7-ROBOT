# IHM_ProjetSEN
Projet (robot mobile) réalisé dans le cadre scolaire pour la matiere de systemes embarqués en semestre 7.

## Constitution trame CAN

* Id emmetteur (PC ou carte)
* Un premier octet de données indiquant le composant à piloter
* Un 2e octet de données pour 

## Communication par UART
Les éléments importants constituant les trames sont:
* un premier octet de données indiquant la zone du robot concernée (roues, tourelle, consigne)
* un 2e octet de données comportant la valeur 

```
void MainWindow::onSliderValueChanged(int value)
{
    qDebug()<<"Callback Slider";
    int32_t convertValue = (int32_t)value;
    int8_t poidsFort = (convertValue && 0x1111111100000000)>>8;
    int8_t poidsFaible = convertValue &&0x11111111;
    sendRobotData(ID_CONSIGNE,poidsFort,poidsFaible); // Pas besoin de les cast en char => même taille
}
```
