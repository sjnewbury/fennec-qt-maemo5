/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* ***** BEGIN LICENSE BLOCK *****
 * Version: MPL 1.1/GPL 2.0/LGPL 2.1
 *
 * The contents of this file are subject to the Mozilla Public License Version
 * 1.1 (the "License"); you may not use this file except in compliance with
 * the License. You may obtain a copy of the License at
 * http://www.mozilla.org/MPL/
 *
 * Software distributed under the License is distributed on an "AS IS" basis,
 * WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
 * for the specific language governing rights and limitations under the
 * License.
 *
 * The Original Code is mozilla.org code.
 *
 * The Initial Developer of the Original Code is
 * Mozilla Foundation.
 * Portions created by the Initial Developer are Copyright (C) 2006
 * the Initial Developer. All Rights Reserved.
 *
 * Contributor(s):
 *   Alexander Surkov <surkov.alexander@gmail.com> (original author)
 *
 * Alternatively, the contents of this file may be used under the terms of
 * either of the GNU General Public License Version 2 or later (the "GPL"),
 * or the GNU Lesser General Public License Version 2.1 or later (the "LGPL"),
 * in which case the provisions of the GPL or the LGPL are applicable instead
 * of those above. If you wish to allow use of your version of this file only
 * under the terms of either the GPL or the LGPL, and not to allow others to
 * use your version of this file under the terms of the MPL, indicate your
 * decision by deleting the provisions above and replace them with the notice
 * and other provisions required by the GPL or the LGPL. If you do not delete
 * the provisions above, a recipient may use your version of this file under
 * the terms of any one of the MPL, the GPL or the LGPL.
 *
 * ***** END LICENSE BLOCK ***** */

#include "nsXFormsFormControlsAccessible.h"

// nsXFormsLabelAccessible

nsXFormsLabelAccessible::
  nsXFormsLabelAccessible(nsIDOMNode *aNode, nsIWeakReference *aShell):
  nsXFormsAccessible(aNode, aShell)
{
}

NS_IMETHODIMP
nsXFormsLabelAccessible::GetRole(PRUint32 *aRole)
{
  NS_ENSURE_ARG_POINTER(aRole);

  *aRole = ROLE_STATICTEXT;
  return NS_OK;
}

NS_IMETHODIMP
nsXFormsLabelAccessible::GetName(nsAString& aName)
{
  nsAutoString name;
  nsresult rv = GetTextFromRelationID(nsAccessibilityAtoms::labelledby, name);
  aName = name;
  return rv;
}

NS_IMETHODIMP
nsXFormsLabelAccessible::GetDescription(nsAString& aDescription)
{
  nsAutoString description;
  nsresult rv = GetTextFromRelationID(nsAccessibilityAtoms::describedby,
                                      description);
  aDescription = description;
  return rv;
}

// nsXFormsOutputAccessible

nsXFormsOutputAccessible::
  nsXFormsOutputAccessible(nsIDOMNode *aNode, nsIWeakReference *aShell):
  nsXFormsAccessible(aNode, aShell)
{
}

NS_IMETHODIMP
nsXFormsOutputAccessible::GetRole(PRUint32 *aRole)
{
  NS_ENSURE_ARG_POINTER(aRole);

  *aRole = ROLE_STATICTEXT;
  return NS_OK;
}

// nsXFormsTriggerAccessible

nsXFormsTriggerAccessible::
  nsXFormsTriggerAccessible(nsIDOMNode *aNode, nsIWeakReference *aShell):
  nsXFormsAccessible(aNode, aShell)
{
}

NS_IMETHODIMP
nsXFormsTriggerAccessible::GetRole(PRUint32 *aRole)
{
  NS_ENSURE_ARG_POINTER(aRole);

  *aRole = ROLE_PUSHBUTTON;
  return NS_OK;
}

NS_IMETHODIMP
nsXFormsTriggerAccessible::GetValue(nsAString& aValue)
{
  aValue.Truncate();
  return NS_OK;
}

NS_IMETHODIMP
nsXFormsTriggerAccessible::GetNumActions(PRUint8 *aCount)
{
  NS_ENSURE_ARG_POINTER(aCount);

  *aCount = 1;
  return NS_OK;
}

NS_IMETHODIMP
nsXFormsTriggerAccessible::GetActionName(PRUint8 aIndex, nsAString& aName)
{
  if (aIndex == eAction_Click) {
    nsAccessible::GetTranslatedString(NS_LITERAL_STRING("press"), aName);
    return NS_OK;
  }
  return NS_ERROR_INVALID_ARG;
}

NS_IMETHODIMP
nsXFormsTriggerAccessible::DoAction(PRUint8 aIndex)
{
  if (aIndex == eAction_Click)
    return DoCommand();

  return NS_ERROR_INVALID_ARG;
}

// nsXFormsInputAccessible

nsXFormsInputAccessible::
  nsXFormsInputAccessible(nsIDOMNode *aNode, nsIWeakReference *aShell):
  nsXFormsAccessible(aNode, aShell)
{
}

NS_IMETHODIMP
nsXFormsInputAccessible::GetRole(PRUint32 *aRole)
{
  NS_ENSURE_ARG_POINTER(aRole);

  *aRole = ROLE_ENTRY;
  return NS_OK;
}

NS_IMETHODIMP
nsXFormsInputAccessible::GetExtState(PRUint32 *aExtState)
{
  NS_ENSURE_ARG_POINTER(aExtState);

  *aExtState = 0;

  NS_ENSURE_TRUE(mDOMNode, NS_ERROR_FAILURE);
  nsresult rv = nsXFormsAccessible::GetExtState(aExtState);
  NS_ENSURE_SUCCESS(rv, rv);

  PRBool state = PR_FALSE;
  rv = sXFormsService->IsReadonly(mDOMNode, &state);
  NS_ENSURE_SUCCESS(rv, rv);

  if (!state) {
    rv = sXFormsService->IsRelevant(mDOMNode, &state);
    NS_ENSURE_SUCCESS(rv, rv);
    if (state) {
      *aExtState |= EXT_STATE_EDITABLE | EXT_STATE_SELECTABLE_TEXT;
    }
  }

  nsCOMPtr<nsIContent> content(do_QueryInterface(mDOMNode));
  if (content->NodeInfo()->Equals(nsAccessibilityAtoms::textarea))
    *aExtState |= EXT_STATE_MULTI_LINE;
  else
    *aExtState |= EXT_STATE_SINGLE_LINE;
}

NS_IMETHODIMP
nsXFormsInputAccessible::GetNumActions(PRUint8* aCount)
{
  NS_ENSURE_ARG_POINTER(aCount);

  *aCount = 1;
  return NS_OK;
}

NS_IMETHODIMP
nsXFormsInputAccessible::GetActionName(PRUint8 aIndex, nsAString& aName)
{
  if (aIndex != eAction_Click)
    return NS_ERROR_INVALID_ARG;

  nsAccessible::GetTranslatedString(NS_LITERAL_STRING("activate"), aName);
  return NS_OK;
}

NS_IMETHODIMP
nsXFormsInputAccessible::DoAction(PRUint8 aIndex)
{
  if (aIndex != eAction_Click)
    return NS_ERROR_INVALID_ARG;

  return sXFormsService->Focus(mDOMNode);
}

// nsXFormsInputBooleanAccessible

nsXFormsInputBooleanAccessible::
  nsXFormsInputBooleanAccessible(nsIDOMNode *aNode, nsIWeakReference *aShell):
  nsXFormsAccessible(aNode, aShell)
{
}

NS_IMETHODIMP
nsXFormsInputBooleanAccessible::GetRole(PRUint32 *aRole)
{
  NS_ENSURE_ARG_POINTER(aRole);

  *aRole = ROLE_CHECKBUTTON;
  return NS_OK;
}

NS_IMETHODIMP
nsXFormsInputBooleanAccessible::GetState(PRUint32 *aState)
{
  nsresult rv = nsXFormsAccessible::GetState(aState);
  NS_ENSURE_SUCCESS(rv, rv);

  nsAutoString value;
  rv = sXFormsService->GetValue(mDOMNode, value);
  NS_ENSURE_SUCCESS(rv, rv);

  if (value.EqualsLiteral("true"))
    *aState |= STATE_CHECKED;

  return NS_OK;
}

NS_IMETHODIMP
nsXFormsInputBooleanAccessible::GetNumActions(PRUint8 *aCount)
{
  NS_ENSURE_ARG_POINTER(aCount);

  *aCount = 1;
  return NS_OK;
}

NS_IMETHODIMP
nsXFormsInputBooleanAccessible::GetActionName(PRUint8 aIndex, nsAString& aName)
{
  if (aIndex != eAction_Click)
    return NS_ERROR_INVALID_ARG;

  nsAutoString value;
  nsresult rv = sXFormsService->GetValue(mDOMNode, value);
  NS_ENSURE_SUCCESS(rv, rv);

  if (value.EqualsLiteral("true"))
    nsAccessible::GetTranslatedString(NS_LITERAL_STRING("uncheck"), aName);
  else
    nsAccessible::GetTranslatedString(NS_LITERAL_STRING("check"), aName);

  return NS_OK;
}

NS_IMETHODIMP
nsXFormsInputBooleanAccessible::DoAction(PRUint8 aIndex)
{
  if (aIndex != eAction_Click)
    return NS_ERROR_INVALID_ARG;

  return DoCommand();
}

// nsXFormsSecretAccessible

nsXFormsSecretAccessible::
  nsXFormsSecretAccessible(nsIDOMNode *aNode, nsIWeakReference *aShell):
  nsXFormsInputAccessible(aNode, aShell)
{
}

NS_IMETHODIMP
nsXFormsSecretAccessible::GetRole(PRUint32 *aRole)
{
  NS_ENSURE_ARG_POINTER(aRole);

  *aRole = ROLE_PASSWORD_TEXT;
  return NS_OK;
}

NS_IMETHODIMP
nsXFormsSecretAccessible::GetState(PRUint32 *aState)
{
  nsresult rv = nsXFormsInputAccessible::GetState(aState);
  NS_ENSURE_SUCCESS(rv, rv);

  *aState |= STATE_PROTECTED;
  return NS_OK;
}

NS_IMETHODIMP
nsXFormsSecretAccessible::GetValue(nsAString& aValue)
{
  return NS_ERROR_FAILURE;
}


// nsXFormsRangeAccessible

nsXFormsRangeAccessible::
  nsXFormsRangeAccessible(nsIDOMNode *aNode, nsIWeakReference *aShell):
  nsXFormsAccessible(aNode, aShell)
{
}

NS_IMETHODIMP
nsXFormsRangeAccessible::GetRole(PRUint32 *aRole)
{
  NS_ENSURE_ARG_POINTER(aRole);

  *aRole = ROLE_SLIDER;
  return NS_OK;
}

NS_IMETHODIMP
nsXFormsRangeAccessible::GetState(PRUint32 *aState)
{
  nsresult rv = nsXFormsAccessible::GetState(aState);
  NS_ENSURE_SUCCESS(rv, rv);

  PRUint32 isInRange = nsIXFormsUtilityService::STATE_NOT_A_RANGE;
  rv = sXFormsService->IsInRange(mDOMNode, &isInRange);
  NS_ENSURE_SUCCESS(rv, rv);

  if (isInRange == nsIXFormsUtilityService::STATE_OUT_OF_RANGE)
    *aState |= STATE_INVALID;

  return NS_OK;
}

NS_IMETHODIMP
nsXFormsRangeAccessible::GetMaximumValue(double *aMaximumValue)
{
  NS_ENSURE_ARG_POINTER(aMaximumValue);

  nsAutoString value;
  nsresult rv = sXFormsService->GetRangeEnd(mDOMNode, value);
  NS_ENSURE_SUCCESS(rv, rv);

  PRInt32 error = NS_OK;
  *aMaximumValue = value.ToFloat(&error);
  return error;
}

NS_IMETHODIMP
nsXFormsRangeAccessible::GetMinimumValue(double *aMinimumValue)
{
  NS_ENSURE_ARG_POINTER(aMinimumValue);

  nsAutoString value;
  nsresult rv = sXFormsService->GetRangeStart(mDOMNode, value);
  NS_ENSURE_SUCCESS(rv, rv);

  PRInt32 error = NS_OK;
  *aMinimumValue = value.ToFloat(&error);
  return error;
}

NS_IMETHODIMP
nsXFormsRangeAccessible::GetMinimumIncrement(double *aMinimumIncrement)
{
  NS_ENSURE_ARG_POINTER(aMinimumIncrement);

  nsAutoString value;
  nsresult rv = sXFormsService->GetRangeStep(mDOMNode, value);
  NS_ENSURE_SUCCESS(rv, rv);

  PRInt32 error = NS_OK;
  *aMinimumIncrement = value.ToFloat(&error);
  return error;
}

NS_IMETHODIMP
nsXFormsRangeAccessible::GetCurrentValue(double *aCurrentValue)
{
  NS_ENSURE_ARG_POINTER(aCurrentValue);

  nsAutoString value;
  nsresult rv = sXFormsService->GetValue(mDOMNode, value);
  NS_ENSURE_SUCCESS(rv, rv);

  PRInt32 error = NS_OK;
  *aCurrentValue = value.ToFloat(&error);
  return error;
}

