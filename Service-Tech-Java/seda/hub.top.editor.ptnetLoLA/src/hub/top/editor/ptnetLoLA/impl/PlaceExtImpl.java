/**
 *  Copyright (c) 2008 Dirk Fahland. All rights reserved. EPL1.0/GPL3.0/LGPL3.0
 *  
 *  ServiceTechnolog.org - PetriNet Editor Framework
 *  
 *  This program and the accompanying materials are made available under
 *  the terms of the Eclipse Public License v1.0, which accompanies this
 *  distribution, and is available at http://www.eclipse.org/legal/epl-v10.html
 *  
 *  Software distributed under the License is distributed on an "AS IS" basis,
 *  WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
 *  for the specific language governing rights and limitations under the
 *  License.
 * 
 *  The Original Code is this file as it was released on July 30, 2008.
 *  The Initial Developer of the Original Code is
 *  		Dirk Fahland
 *  
 *  Portions created by the Initial Developer are Copyright (c) 2008
 *  the Initial Developer. All Rights Reserved.
 * 
 *  Contributor(s):
 * 
 *  Alternatively, the contents of this file may be used under the terms of
 *  either the GNU General Public License Version 3 or later (the "GPL"), or
 *  the GNU Lesser General Public License Version 3 or later (the "LGPL"),
 *  in which case the provisions of the GPL or the LGPL are applicable instead
 *  of those above. If you wish to allow use of your version of this file only
 *  under the terms of either the GPL or the LGPL, and not to allow others to
 *  use your version of this file under the terms of the EPL, indicate your
 *  decision by deleting the provisions above and replace them with the notice
 *  and other provisions required by the GPL or the LGPL. If you do not delete
 *  the provisions above, a recipient may use your version of this file under
 *  the terms of any one of the EPL, the GPL or the LGPL.
 *
 * $Id$
 */
package hub.top.editor.ptnetLoLA.impl;

import hub.top.editor.ptnetLoLA.PlaceExt;
import hub.top.editor.ptnetLoLA.PtnetLoLAPackage;

import org.eclipse.emf.common.notify.Notification;

import org.eclipse.emf.ecore.EClass;

import org.eclipse.emf.ecore.impl.ENotificationImpl;

/**
 * <!-- begin-user-doc -->
 * An implementation of the model object '<em><b>Place Ext</b></em>'.
 * <!-- end-user-doc -->
 * <p>
 * The following features are implemented:
 * <ul>
 *   <li>{@link hub.top.editor.ptnetLoLA.impl.PlaceExtImpl#getProbability <em>Probability</em>}</li>
 *   <li>{@link hub.top.editor.ptnetLoLA.impl.PlaceExtImpl#isIsStart <em>Is Start</em>}</li>
 * </ul>
 * </p>
 *
 * @generated
 */
public class PlaceExtImpl extends PlaceImpl implements PlaceExt {
  /**
   * The default value of the '{@link #getProbability() <em>Probability</em>}' attribute.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @see #getProbability()
   * @generated
   * @ordered
   */
  protected static final double PROBABILITY_EDEFAULT = 1.0;

  /**
   * The cached value of the '{@link #getProbability() <em>Probability</em>}' attribute.
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @see #getProbability()
   * @generated
   * @ordered
   */
  protected double probability = PROBABILITY_EDEFAULT;

  /**
   * The default value of the '{@link #isIsStart() <em>Is Start</em>}' attribute.
   * <!-- begin-user-doc -->
	 * <!-- end-user-doc -->
   * @see #isIsStart()
   * @generated
   * @ordered
   */
	protected static final boolean IS_START_EDEFAULT = false;

		/**
   * The cached value of the '{@link #isIsStart() <em>Is Start</em>}' attribute.
   * <!-- begin-user-doc -->
	 * <!-- end-user-doc -->
   * @see #isIsStart()
   * @generated
   * @ordered
   */
	protected boolean isStart = IS_START_EDEFAULT;

		/**
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   */
  protected PlaceExtImpl() {
    super();
  }

  /**
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   */
  @Override
  protected EClass eStaticClass() {
    return PtnetLoLAPackage.Literals.PLACE_EXT;
  }

  /**
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   */
  public double getProbability() {
    return probability;
  }

  /**
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   */
  public void setProbability(double newProbability) {
    double oldProbability = probability;
    probability = newProbability;
    if (eNotificationRequired())
      eNotify(new ENotificationImpl(this, Notification.SET, PtnetLoLAPackage.PLACE_EXT__PROBABILITY, oldProbability, probability));
  }

  /**
   * <!-- begin-user-doc -->
	 * <!-- end-user-doc -->
   * @generated
   */
	public boolean isIsStart() {
    return isStart;
  }

		/**
   * <!-- begin-user-doc -->
	 * <!-- end-user-doc -->
   * @generated
   */
	public void setIsStart(boolean newIsStart) {
    boolean oldIsStart = isStart;
    isStart = newIsStart;
    if (eNotificationRequired())
      eNotify(new ENotificationImpl(this, Notification.SET, PtnetLoLAPackage.PLACE_EXT__IS_START, oldIsStart, isStart));
  }

		/**
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   */
  @Override
  public Object eGet(int featureID, boolean resolve, boolean coreType) {
    switch (featureID) {
      case PtnetLoLAPackage.PLACE_EXT__PROBABILITY:
        return getProbability();
      case PtnetLoLAPackage.PLACE_EXT__IS_START:
        return isIsStart();
    }
    return super.eGet(featureID, resolve, coreType);
  }

  /**
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   */
  @Override
  public void eSet(int featureID, Object newValue) {
    switch (featureID) {
      case PtnetLoLAPackage.PLACE_EXT__PROBABILITY:
        setProbability((Double)newValue);
        return;
      case PtnetLoLAPackage.PLACE_EXT__IS_START:
        setIsStart((Boolean)newValue);
        return;
    }
    super.eSet(featureID, newValue);
  }

  /**
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   */
  @Override
  public void eUnset(int featureID) {
    switch (featureID) {
      case PtnetLoLAPackage.PLACE_EXT__PROBABILITY:
        setProbability(PROBABILITY_EDEFAULT);
        return;
      case PtnetLoLAPackage.PLACE_EXT__IS_START:
        setIsStart(IS_START_EDEFAULT);
        return;
    }
    super.eUnset(featureID);
  }

  /**
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   */
  @Override
  public boolean eIsSet(int featureID) {
    switch (featureID) {
      case PtnetLoLAPackage.PLACE_EXT__PROBABILITY:
        return probability != PROBABILITY_EDEFAULT;
      case PtnetLoLAPackage.PLACE_EXT__IS_START:
        return isStart != IS_START_EDEFAULT;
    }
    return super.eIsSet(featureID);
  }

  /**
   * <!-- begin-user-doc -->
   * <!-- end-user-doc -->
   * @generated
   */
  @Override
  public String toString() {
    if (eIsProxy()) return super.toString();

    StringBuffer result = new StringBuffer(super.toString());
    result.append(" (probability: ");
    result.append(probability);
    result.append(", isStart: ");
    result.append(isStart);
    result.append(')');
    return result.toString();
  }

} //PlaceExtImpl
