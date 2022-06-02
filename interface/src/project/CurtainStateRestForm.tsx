import { FC } from 'react';

import { Button, Checkbox } from '@mui/material';
import SaveIcon from '@mui/icons-material/Save';

import { SectionContent, FormLoader, BlockFormControlLabel, ButtonRow, MessageBox } from '../components';
import { updateValue, useRest } from '../utils';

import * as DemoApi from './api';
import { CurtainState } from './types';

const CurtainStateRestForm: FC = () => {
  const {
    loadData, saveData, saving, setData, data, errorMessage
  } = useRest<CurtainState>({ read: DemoApi.readCurtainState, update: DemoApi.updateCurtainState });

  const updateFormValue = updateValue(setData);

  const content = () => {
    if (!data) {
      return (<FormLoader onRetry={loadData} errorMessage={errorMessage} />);
    }

    return (
      <>
        <MessageBox
          level="info"
          message="Schaltet den Wert closed via RESTful Service vom ESP"
          my={2}
        />
        <BlockFormControlLabel
          control={
            <Checkbox
              name="closed"
              disabled={saving}
              checked={data.closed}
              onChange={updateFormValue}
              color="primary"
            />
          }
          label="Closed State?"
        />
        <ButtonRow mt={1}>
          <Button startIcon={<SaveIcon />} disabled={saving} variant="contained" color="primary" type="submit" onClick={saveData}>
            Save
          </Button>
        </ButtonRow>
      </>
    );
  };

  return (
    <SectionContent title='REST Example' titleGutter>
      {content()}
    </SectionContent>
  );
};

export default CurtainStateRestForm;
