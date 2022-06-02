import { FC } from 'react';

import { Typography } from '@mui/material';

import { SectionContent } from '../components';

const CurtainInformation: FC = () => (
  <SectionContent title='Curtain Information' titleGutter>
    <Typography variant="body1" paragraph>
      Vorhangsteuerung
    </Typography>

  </SectionContent>
);

export default CurtainInformation;
